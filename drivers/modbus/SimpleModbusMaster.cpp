/**
  ******************************************************************************
  * @file    SimpleModbusMaster.h 
  * @author  Multicalor
  * @version V1.0.0
  * @date    23-01-2015
  * @brief   Header for SimpleModbusMaster.c module
	@brief	Verventa 1.0.0
  ******************************************************************************
  * @attention
  *
  * SimpleModbusMasterV10
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  * SimpleModbusMaster allows you to communicate
  * to any slave using the Modbus RTU protocol.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <cstdio>
#include "SimpleModbusMaster.h"
#include "stm32f4xx_it.h"
/* Defines -------------------------------------------------------------------*/
#define IDLE 1
#define WAITING_FOR_REPLY 2
#define WAITING_FOR_TURNAROUND 3

#define BUFFER_SIZE 256

extern UART_HandleTypeDef UartHandle;

/* Private variables --------------------------------------------------------*/
static uint8_t state;
uint8_t retry_count;
// frame[] is used to receive and transmit packages. 
// The maximum number of bytes in a modbus packet is 256 bytes
uint8_t frame[BUFFER_SIZE]; 
uint8_t buffer;
uint16_t timeout; // timeout interval
uint16_t polling; // turnaround delay interval
uint16_t T1_5; // inter character time out in microseconds
static uint32_t delayStart; // init variable for turnaround and timeout delay
uint16_t total_no_of_packets; 
packetPointer packetArray; // packet starting address
packetPointer packet; // current packet

uint8_t aTxBuffer_Usart[BUFFER_SIZE];
uint8_t aRxBuffer_Usart[RXBUFFERSIZEUSART];

__IO ITStatus UartReady = RESET;

/* Private Functions -----------------------------------------------------*/
static void idle();
static void constructPacket();
static uint8_t construct_F15();
static uint8_t construct_F16();
static void waiting_for_reply();
static void processReply();
static void waiting_for_turnaround();
static void process_F1_F2();
static void process_F3_F4();
static void process_F15_F16();
static void processError();
static void processSuccess();
static uint16_t calculateCRC(uint8_t bufferSize);
static void sendPacket(uint8_t bufferSize);

/***************************************************************************/
/*	Modbus Master	-------------------------------------------------------*/
/**
  * @brief  update the state machine
  * @param  none 
  * @note   3 states: idle, waiting reply, waiting turnaround
  * @retval None
  */
void modbus_update(){
	//std::printf("state: %d\n", state);
	switch (state)
	{
		case IDLE:
		idle();
		break;
		case WAITING_FOR_REPLY:
		waiting_for_reply();
		break;
		case WAITING_FOR_TURNAROUND:
		waiting_for_turnaround();
		break;
	}
}

/**
  * @brief  Idle state
  * @param  none 
  * @note   prepare and send a packet
  * @retval None
  */
void idle()
{
	static uint16_t packet_index;
	uint16_t failed_connections = 0;
	uint8_t current_connection;
	
	do
	{		
		if (packet_index == total_no_of_packets) // wrap around to the beginning
			packet_index = 0;
				
		// proceed to the next packet
		packet = &packetArray[packet_index];
		
		// get the current connection status
		current_connection = packet->connection;
		
		if (!current_connection)
		{			
			// If all the connection attributes are false return
			// immediately to the main sketch
			if (++failed_connections == total_no_of_packets)
				return;
		}
		packet_index++;     
    
	// if a packet has no connection get the next one		
	}while (!current_connection); 
		
	constructPacket();
}
  
void constructPacket()
{
  uint8_t frameSize;
  
  packet->requests++;
  frame[0] = packet->id;
  frame[1] = packet->function;
  frame[2] = packet->address >> 8; // address Hi
  frame[3] = packet->address & 0xFF; // address Lo
  // For functions 1 & 2 data is the number of points
  // For functions 3, 4 & 16 data is the number of registers
  // For function 15 data is the number of coils
  frame[4] = packet->data >> 8; // MSB
  frame[5] = packet->data & 0xFF; // LSB
	
  // construct the frame according to the modbus function  
  if (packet->function == PRESET_MULTIPLE_REGISTERS) 
		frameSize = construct_F16();
	else if (packet->function == FORCE_MULTIPLE_COILS)
		frameSize = construct_F15();
	else // else functions 1,2,3 & 4 is assumed. They all share the exact same request format.
    frameSize = 8; // the request is always 8 bytes in size for the above mentioned functions.
		
  uint16_t crc16 = calculateCRC(frameSize - 2);	
  frame[frameSize - 2] = crc16 >> 8; // split crc into 2 bytes
  frame[frameSize - 1] = crc16 & 0xFF;
  sendPacket(frameSize);

  state = WAITING_FOR_REPLY; // state change
	
  // if broadcast is requested (id == 0) for function 15 or 16 then override 
  // the previous state and force a success since the slave wont respond
  if (packet->id == 0)
		processSuccess();
}

uint8_t construct_F15()
{
  // function 15 coil information is packed LSB first until the first 16 bits are completed
  // It is received the same way..
  uint8_t no_of_registers = packet->data / 16;
  uint8_t no_of_bytes = no_of_registers * 2;
  uint8_t bytes_processed = 0;
  uint8_t index = 7; // user data starts at index 7
  uint16_t temp;
	
  // if the number of points dont fit in even 2byte amounts (one register) then use another register and pad 
  if (packet->data % 16 > 0) 
  {
    no_of_registers++;
    no_of_bytes++;
  }
	
  frame[6] = no_of_bytes;
	
  for(uint8_t i = 0; i < no_of_registers; i++)
  {
    temp = packet->register_array[i]; // get the data
    frame[index] = temp & 0xFF; 
    bytes_processed++;
     
    if (bytes_processed < no_of_bytes)
    {
      frame[index + 1] = temp >> 8;
      bytes_processed++;
      index += 2;
    }
  }
	uint8_t frameSize = (9 + no_of_bytes); // first 7 bytes of the array + 2 bytes CRC + noOfBytes 
	return frameSize;
}

uint8_t construct_F16()
{
	uint8_t no_of_bytes = packet->data * 2; 
    
	// first 6 bytes of the array + no_of_bytes + 2 bytes CRC 
	frame[6] = no_of_bytes; // number of bytes
	uint8_t index = 7; // user data starts at index 7
	uint8_t no_of_registers = packet->data;
	uint16_t temp;

  for(uint8_t i = 0; i < no_of_registers; i++)
  {
    temp = packet->register_array[i]; // get the data
    frame[index] = temp >> 8;
    index++;
    frame[index] = temp & 0xFF;
    index++;
  }
	uint8_t frameSize = (9 + no_of_bytes); // first 7 bytes of the array + 2 bytes CRC + noOfBytes 
	return frameSize;
}

void waiting_for_turnaround()
{
  if ((timeout_timer() - delayStart) > polling)
		state = IDLE;
}

/**
  * @brief  wait for reply state
  * @param  none 
  * @note   wait a reply from requested slave
  * @retval None
  */
void waiting_for_reply()
{
	buffer = 0;
		
	if(__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_RXNE) != RESET){
		std::printf("RX not empty...\n");

		if(HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)aRxBuffer_Usart, 20) != HAL_OK){
			printf("USART Receive Error\n");
		}
		/*else{
			//state = IDLE;
			for(buffer = 0; buffer < 22; buffer++){
				frame[buffer] = aRxBuffer_Usart[buffer];
				std::printf("ReceiveBuffer: %d\n", frame[buffer]);
			}
		}*/
	}
	else if(UartReady != RESET){
		for(buffer = 0; buffer < 12; buffer++){
			frame[buffer] = aRxBuffer_Usart[buffer];
			std::printf("ReceiveBuffer: %d\n", frame[buffer]);
		}
		UartReady = RESET;

		if(frame[0] = packet->id){
			processReply();
		}
	}
	// The minimum buffer size from a slave can be an exception response of
	// 5 bytes. If the buffer was partially filled set a frame_error.
	// The maximum number of bytes in a modbus packet is 256 bytes.
	// The serial buffer limits this to 128 bytes.
	
	/*if(buffer < 5)
		processError();*/       
      
	// Modbus over serial line datasheet states that if an unexpected slave 
	// responded the master must do nothing and continue with the time out.
	// This seems silly cause if an incorrect slave responded you would want to
	// have a quick turnaround and poll the right one again. If an unexpected 
	// slave responded it will most likely be a frame error in any event
	/*else if(frame[0] != packet->id)
		processError();*/
	/*else
		processReply();*/

	else if((timeout_timer() - delayStart) > timeout){
		std::printf("TimeOut: %d\n", (timeout_timer() - delayStart));
		processError();
		state = IDLE; //state change, override processError() state
	}
}

void processReply()
{
  // combine the crc Low & High bytes
  uint16_t received_crc = ((frame[buffer - 2] << 8) | frame[buffer - 1]); 
  uint16_t calculated_crc = calculateCRC(buffer - 2);
	
	// verify checksum
	if(calculated_crc == received_crc){
		// To indicate an exception response a slave will 'OR' 
		// the requested function with 0x80
		if((frame[1] & 0x80) == 0x80){
			packet->exception_errors++;
			processError();
		}
		else{
			// check function returned
			switch (frame[1]){
				case READ_COIL_STATUS:
				case READ_INPUT_STATUS:
				process_F1_F2();
				break;
				case READ_INPUT_REGISTERS:
				case READ_HOLDING_REGISTERS:
				process_F3_F4();
				break;
				case FORCE_MULTIPLE_COILS:
				case PRESET_MULTIPLE_REGISTERS:
				process_F15_F16();
				break;
				default: // illegal function returned
				processError();
				break;
			}
		}
	} 
	else // checksum failed
	{
		processError();
	}
}

void process_F1_F2()
{
	// packet->data for function 1 & 2 is actually the number of boolean points
  uint8_t no_of_registers = packet->data / 16;
  uint8_t number_of_bytes = no_of_registers * 2; 
       
  // if the number of points dont fit in even 2byte amounts (one register) then use another register and pad 
  if(packet->data % 16 > 0) 
  {
    no_of_registers++;
    number_of_bytes++;
  }
             
  if (frame[2] == number_of_bytes) // check number of bytes returned
  { 
    uint8_t bytes_processed = 0;
    uint8_t index = 3; // start at the 4th element in the frame and combine the Lo byte  
    uint16_t temp;
    for(uint8_t i = 0; i < no_of_registers; i++)
    {
      temp = frame[index]; 
      bytes_processed++;
      if(bytes_processed < number_of_bytes)
      {
				temp = (frame[index + 1] << 8) | temp;
        bytes_processed++;
        index += 2;
      }
      packet->register_array[i] = temp;
    }
    processSuccess(); 
  }
  else // incorrect number of bytes returned 
    processError();
}

void process_F3_F4()
{
  // check number of bytes returned - unsigned int == 2 bytes
  // data for function 3 & 4 is the number of registers
  if (frame[2] == (packet->data * 2)) 
  {
    uint8_t index = 3;
    for(uint8_t i = 0; i < packet->data; i++)
    {
      // start at the 4th element in the frame and combine the Lo byte 
      packet->register_array[i] = (frame[index] << 8) | frame[index + 1]; 
      index += 2;
    }
    processSuccess(); 
  }
  else // incorrect number of bytes returned  
    processError();  
}

void process_F15_F16()
{
	// Functions 15 & 16 is just an echo of the query
  uint16_t recieved_address = ((frame[2] << 8) | frame[3]);
  uint16_t recieved_data = ((frame[4] << 8) | frame[5]);
		
  if ((recieved_address == packet->address) && (recieved_data == packet->data))
    processSuccess();
  else
    processError();
}

void processError()
{
	packet->retries++;
	packet->failed_requests++;
	
	// if the number of retries have reached the max number of retries 
	// allowable, stop requesting the specific packet
	if (packet->retries == retry_count){
		packet->connection = 0;
		packet->retries = 0;
	}
	state = WAITING_FOR_TURNAROUND;
	delayStart = timeout_timer(); // start the turnaround delay
}

void processSuccess()
{
	packet->successful_requests++; // transaction sent successfully
	packet->retries = 0; // if a request was successful reset the retry counter
	state = WAITING_FOR_TURNAROUND;
	delayStart = timeout_timer(); // start the turnaround delay
}

/**
  * @brief  configure a modbus Port
  * @param  uint32_t: baud, uint16_t: _timeout, uint16_t: _polling, uint8_t: _retry_count, Packet*: _packets, uint16_t: _total_no_of_packets
  * @note
  * @retval none
  */
void modbus_configure(	uint32_t baud,
				uint16_t _timeout, 
				uint16_t _polling,
				uint8_t _retry_count,
				Packet* _packets, 
				uint16_t _total_no_of_packets)
{ 
  // Modbus states that a baud rate higher than 19200 must use a fixed 750 us 
  // for inter character time out and 1.75 ms for a frame delay for baud rates
  // below 19200 the timing is more critical and has to be calculated.
  // E.g. 9600 baud in a 11 bit packet is 9600/11 = 872 characters per second
  // In milliseconds this will be 872 characters per 1000ms. So for 1 character
  // 1000ms/872 characters is 1.14583ms per character and finally modbus states
  // an inter-character must be 1.5T or 1.5 times longer than a character. Thus
  // 1.5T = 1.14583ms * 1.5 = 1.71875ms. A frame delay is 3.5T.
  // Thus the formula is T1.5(us) = (1000ms * 1000(us) * 1.5 * 11bits)/baud
  // 1000ms * 1000(us) * 1.5 * 11bits = 16500000 can be calculated as a constant
	
	if (baud > 19200)
		T1_5 = 750;
	else 
		T1_5 = 16500000/baud; // 1T * 1.5 = T1.5
	
	// initialize
	state = IDLE;
	timeout = _timeout;
	polling = _polling;
	retry_count = _retry_count;
	//TxEnablePin = _TxEnablePin;
	total_no_of_packets = _total_no_of_packets;
	packetArray = _packets;
	
	// initialize connection status of each packet
	/*for (unsigned char i = 0; i < total_no_of_packets; i++)
	{
		_packets->connection = 1;
		_packets++;
	}*/
	
} 

/**
  * @brief  construct a modBus packet
  * @param  Packet: _packet, uint8_t: id, uint8_t: function, uint16_t: address, uint16_t: data, uint16_t*: register_array
  * @note
  * @retval none
  */
void modbus_construct(Packet *_packet, 
				uint8_t id, 
				uint8_t function, 
				uint16_t address, 
				uint16_t data, 
				uint16_t* register_array)
{
	_packet->id = id;
	_packet->function = function;
	_packet->address = address;
	_packet->data = data;
	_packet->register_array = register_array;
	_packet->connection = 1;
}

/**
  * @brief  calculate the crc
  * @param  uint8_t: bufferSize. 
  * @note
  * @retval uint16_t: temp
  */
static uint16_t calculateCRC(uint8_t bufferSize){
  uint16_t temp, temp2, flag;
  temp = 0xFFFF;
  for (uint8_t i = 0; i < bufferSize; i++)
  {
    temp = temp ^ frame[i];
    for (uint8_t j = 1; j <= 8; j++)
    {
      flag = temp & 0x0001;
      temp >>= 1;
      if (flag)
        temp ^= 0xA001;
    }
  }
  // Reverse byte order. 
  temp2 = temp >> 8;
  temp = (temp << 8) | temp2;
  temp &= 0xFFFF;
  // the returned value is already swapped
  // crcLo byte is first & crcHi byte is last
  return temp; 
}

/**
  * @brief  send Packet to slave
  * @param  uint8_t: bufferSize. 
  * @note   tx buffer get filled with frame content
  * @retval None
  */
static void sendPacket(uint8_t bufferSize){
	//digitalWrite(TxEnablePin, HIGH);
		
	for (uint8_t i = 0; i < bufferSize; i++){
		aTxBuffer_Usart[i] = frame[i];
		/*std::printf("Transmitbuffer: %d\n", frame[i]);*/
	}
	
	// It may be necessary to add a another character delay T1_5 here to
	// avoid truncating the message on slow and long distance connections
	
	if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)aTxBuffer_Usart, 8)!= HAL_OK){
		printf("USART Transmit Error\n");
	}
		
/*	while (UartReady != SET){
		std::printf("Waiting on bus free!\n");
	}*/
		
	UartReady = RESET;
	
	//digitalWrite(TxEnablePin, LOW);
		
	delayStart = timeout_timer(); // start the timeout delay	
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of DMA Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle){
  /* Set transmission flag: transfer complete */
  UartReady = SET;
  
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){
  /* Set transmission flag: transfer complete */
  UartReady = SET;
  
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle){
  /* Transfer error in reception/transmission process */
  printf("USART bus Error"); 
}
