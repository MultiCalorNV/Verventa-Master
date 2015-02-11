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
  * To communicate with a slave you need to create a packet that will contain 
  * all the information required to communicate to the slave.
  * Information counters are implemented for further diagnostic.
  * These are variables already implemented in a packet. 
  * You can set and clear these variables as needed.
  * The following modbus information counters are implemented:    
  * requests - contains the total requests to a slave
  * successful_requests - contains the total successful requests
  * failed_requests - general frame errors, checksum failures and buffer failures 
  * retries - contains the number of retries
  * exception_errors - contains the specific modbus exception response count
  * These are normally illegal function, illegal address, illegal data value
  * or a miscellaneous error response.
  *
  * And finally there is a variable called "connection" that
  * at any given moment contains the current connection
  * status of the packet. If true then the connection is 
  * active. If false then communication will be stopped
  * on this packet until the programmer sets the connection
  * variable to true explicitly. The reason for this is 
  * because of the time out involved in modbus communication.
  * Each faulty slave that's not communicating will slow down
  * communication on the line with the time out value. E.g.
  * Using a time out of 1500ms, if you have 10 slaves and 9 of them
  * stops communicating the latency burden placed on communication
  * will be 1500ms * 9 = 13,5 seconds!  
  * Communication will automatically be stopped after the retry count expires
  * on each specific packet.
  *
  * All the error checking, updating and communication multitasking
  * takes place in the background.
  *
  * In general to communicate with to a slave using modbus
  * RTU you will request information using the specific
  * slave id, the function request, the starting address
  * and lastly the data to request.
  * Function 1, 2, 3, 4, 15 & 16 are supported. In addition to
  * this broadcasting (id = 0) is supported for function 15 & 16.
  *
  * Constants are provided for:
  *		Function 1  - READ_COIL_STATUS
  *		Function 2  - READ_INPUT_STATUS
  *		Function 3  - READ_HOLDING_REGISTERS 
  *		Function 4  - READ_INPUT_REGISTERS
  *		Function 15 - FORCE_MULTIPLE_COILS
  *		Function 16 - PRESET_MULTIPLE_REGISTERS
  *
  * In a function 3 or 4 request the master will attempt to read from a
  * slave and since 5 bytes is already used for ID, FUNCTION, NO OF BYTES
  * and two BYTES CRC the master can only request 122 bytes or 61 registers.
  *
  * In a function 16 request the master will attempt to write to a 
  * slave and since 9 bytes is already used for ID, FUNCTION, ADDRESS, 
  * NO OF REGISTERS, NO OF BYTES and two BYTES CRC the master can only write
  * 118 bytes or 59 registers.
  ******************************************************************************
  */
#ifndef __SIMPLE_MODBUS_MASTER_H
#define __SIMPLE_MODBUS_MASTER_H

#ifdef __cplusplus
 extern "C" {
#endif

/*	includes------------------------------------------------------------------*/
#include "main.h"
/* Exported constants --------------------------------------------------------*/
#define READ_COIL_STATUS 1 // Reads the ON/OFF status of discrete outputs (0X references, coils) in the slave.
#define READ_INPUT_STATUS 2 // Reads the ON/OFF status of discrete inputs (1X references) in the slave.
#define READ_HOLDING_REGISTERS 3 // Reads the binary contents of holding registers (4X references) in the slave.
#define READ_INPUT_REGISTERS 4 // Reads the binary contents of input registers (3X references) in the slave. Not writable.
#define FORCE_MULTIPLE_COILS 15 // Forces each coil (0X reference) in a sequence of coils to either ON or OFF.
#define PRESET_MULTIPLE_REGISTERS 16 // Presets values into a sequence of holding registers (4X references).

/*	typedefs-----------------------------------------------------------------*/
// For functions 1 & 2 'data' is the number of points
// For functions 3, 4 & 16 'data' is the number of registers
// For function 15 'data' is the number of coils
typedef struct
{
  // specific packet info
  uint8_t id;
  uint8_t function;
  uint16_t address;
  uint16_t data; 
  uint16_t* register_array;
  
  // modbus information counters
  uint16_t requests;
  uint16_t successful_requests;
  uint16_t failed_requests;
  uint16_t exception_errors;
  uint16_t retries;
  	
  // connection status of packet
  uint8_t connection; 
  
}Packet;

typedef Packet* packetPointer;

/*	Exported Functions--------------------------------------------------------*/
void modbus_update();
void modbus_construct(Packet *_packet, 
											uint8_t id, 
											uint8_t function, 
											uint16_t address, 
											uint16_t data, 
											uint16_t* register_array);											
void modbus_configure(	uint32_t baud,
										uint16_t _timeout, 
										uint16_t _polling, 
										uint8_t _retry_count, 
										Packet* _packets, 
										uint16_t _total_no_of_packets);


#ifdef __cplusplus
}
#endif

#endif