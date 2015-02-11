/**
  ******************************************************************************
  * @file    modbusConfig.h 
  * @author  Multicalor
  * @version V1.0.0
  * @date    23-01-2015
  * @brief   Header for modbusConfig.c module
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
  * Configure the packets you want to write or read from slave devices.
  *
  * uint8_t id; -> slave address
  * uint8_t function; -> 1/2/3/4/15/16
  * uint16_t address; -> starting address of register
  * uint16_t data; -> n° of registers to read/write
  * uint16_t* register_array; -> holds the values
  *
  * Save this packets in an array.
  *
  * Run modbus_construct() to instantiate the packets.
  * Run modbus_configure() to pass the array.
  *
  ******************************************************************************
  */
#ifndef __MODBUS_CONFIG_H
#define __MODBUS_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/*	Includes----------------------------------------*/
#include "main.h"
#include "SimpleModbusMaster.h"
/*	Enums-------------------------------------------*/
// Create new packets
enum{
	PACKET1, 
	PACKET2, 
	// more packets
	TOTAL_NO_OF_PACKETS
};
/*	exported variables------------------------------*/
// Packets Array
Packet packets[TOTAL_NO_OF_PACKETS];
// Create the packet pointer to every packet in array
packetPointer packet1 = &packets[PACKET1];
packetPointer packet2 = &packets[PACKET2];
// The data from the PLC will be stored in the regs array
uint16_t regs[200];
/*	exported functions------------------------------*/
void modbus_Setup();

#ifdef __cplusplus
}
#endif

#endif