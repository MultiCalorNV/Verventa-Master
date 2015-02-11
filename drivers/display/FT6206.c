/**
  ******************************************************************************
  * @file    FT6206.h 
  * @author  Multicalor
  * @version V1.0.0
  * @date    23-01-2015
  * @brief   Header for FT6206.c module
	@brief	Verventa 1.0.0
  ******************************************************************************
  * @attention
  *
  * 
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
  ******************************************************************************
  */
#include <stdio.h>

/* Includes ------------------------------------------------------------------*/
#include "FT6206.h"
#include "main.h"
#include "stm32f4xx_it.h"

/* Private variables -------------------------------------------------*/
/* FT6206 Instance */
FT6206_t FT6206;

/* Private variables ---------------------------------------------------------*/
/* I2C handler declared in "main.c" file */
extern I2C_HandleTypeDef I2cHandle;
/* Buffer used for transmission */
uint8_t aTxBuffer_I2C[2];
/* Buffer used for reception */
uint8_t aRxBuffer_I2C[RXBUFFERSIZEI2C];

/* Private Functions -----------------------------------------------------*/
static uint8_t FT6206_read_register(uint8_t reg);
static uint8_t FT6206_write_register(uint8_t reg, uint8_t data);



static uint8_t FT6206_read_register(uint8_t reg){
	uint8_t return_val = 0;
	
	aTxBuffer_I2C[0] = reg;
	/*	REady to transmit?	*/
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
	/* transmit data through "aTxBuffer" buffer */
	if(HAL_I2C_Master_Transmit(&I2cHandle, (uint8_t)FT6206_ADDR<<1, (uint8_t*)aTxBuffer_I2C, (TXBUFFERSIZEI2C -1), 1000)!= HAL_OK){
		/* Error_Handler() function is called when Timeout error occurs.
		When Acknowledge failure occurs (Slave don't acknowledge it's address)
		Master restarts communication */
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		printf("Error\n");
	}
	
	/*	Wait till end of transmit	*/
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
	/* recept data through "aRxBuffer" buffer */
	if(HAL_I2C_Master_Receive(&I2cHandle, (uint8_t)FT6206_ADDR<<1, (uint8_t *)aRxBuffer_I2C, (RXBUFFERSIZEI2C -1), 1000) != HAL_OK){
		/* Error_Handler() function is called when Timeout error occurs.
		When Acknowledge failure occurs (Slave don't acknowledge it's address)
		Master restarts communication */
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		printf("Error\n");
	}
	
	return_val = aRxBuffer_I2C[0];
	
	return return_val;
}

static uint8_t FT6206_write_register(uint8_t reg, uint8_t data){
	uint8_t return_val = 0;
	
	
	aTxBuffer_I2C[0] = reg;
	aTxBuffer_I2C[1] = data;
	/*	REady to transmit?	*/
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
	/* transmit data through "aTxBuffer" buffer */
	if(HAL_I2C_Master_Transmit(&I2cHandle, (uint8_t)FT6206_ADDR<<1, (uint8_t*)aTxBuffer_I2C, TXBUFFERSIZEI2C, 1000)!= HAL_OK){
		/* Error_Handler() function is called when Timeout error occurs.
		When Acknowledge failure occurs (Slave don't acknowledge it's address)
		Master restarts communication */
		printf("Error\n");
	}
	
	return return_val;
}


uint8_t FT6206_begin(uint8_t threshold){
	//  0...255 last byte value read
	uint8_t x = 0;
	
	/*Reset pin 5 msec. low */
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
	delay_msec(5);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
	
	// change threshold to be higher/lower
	FT6206_write_register((uint8_t)FT6206_REG_THRESHOLD, threshold);
	
	FT6206_write_register((uint8_t)FT6206_REG_G_MODE, FT6206_INT_POLLING_MODE);
	
	// fill structure
	FT6206.vendor_id = FT6206_read_register((uint8_t)FT6206_REG_VENDID);
	FT6206.chip_id = FT6206_read_register((uint8_t)FT6206_REG_CHIPID);
	FT6206.firmware_version = FT6206_read_register((uint8_t)FT6206_REG_FIRMVERS);
	FT6206.pointrate = FT6206_read_register((uint8_t)FT6206_REG_POINTRATE);
	FT6206.threshold = FT6206_read_register((uint8_t)FT6206_REG_THRESHOLD);

	  printf("Vendor ID       : %6d\n", FT6206.vendor_id);
	  printf("Chip ID         : %6d\n", FT6206.chip_id);
	  printf("Firmware version: %6d\n", FT6206.firmware_version);
	  printf("Pointrate       : %6d\n", FT6206.pointrate);
	  printf("Threshold       : %6d\n", FT6206.threshold);

  return x;
}

uint8_t FT6206_touched(void){
	//  1...2 touches
	uint8_t n;
	
	n = FT6206_read_register((uint8_t)FT6206_REG_NUMTOUCHES);

	if((n == 1) || (n == 2)){
		return n;
	}
	else{
		return 0;
	}
}

void FT6206_read_data(void){
	// array to hold values
  uint8_t raw_tp_data[16];		// array to hold packed info
  uint8_t i;
  uint8_t x;
  
  for (i=2; i<7; i++){
	  x = FT6206_read_register(i);
  	  raw_tp_data[i] = x;
  }

  //FT6206.gesture_id = raw_tp_data[1];

  FT6206.touch_points = raw_tp_data[2];

  if(FT6206.touch_points > 2){	// error! only 0/1/2 are valid
  	FT6206.touch_points = 0;
  	FT6206.touch_01_xpos = 0;
  	FT6206.touch_01_ypos = 0;
  	FT6206.touch_01_weight = 0;
  	FT6206.touch_01_area = 0;
  	FT6206.touch_02_xpos = 0;
  	FT6206.touch_02_ypos = 0;
  	FT6206.touch_02_weight = 0;
  	FT6206.touch_02_area = 0;
  	return;
  }
  if(FT6206.touch_points == 0){	// error! only 0/1/2 are valid
  	FT6206.touch_points = 0;
  	FT6206.touch_01_xpos = 0;
  	FT6206.touch_01_ypos = 0;
  	FT6206.touch_01_weight = 0;
  	FT6206.touch_01_area = 0;
  	FT6206.touch_02_xpos = 0;
  	FT6206.touch_02_ypos = 0;
  	FT6206.touch_02_weight = 0;
  	FT6206.touch_02_area = 0;
    return;
  }

  FT6206.touch_01_xpos = ((raw_tp_data[3]) & 0x0F);	// Mask low nibble of byte
  FT6206.touch_01_xpos <<= 8;					// left shift 8
  FT6206.touch_01_xpos |= raw_tp_data[4];		// OR with reg(0x04)
  
  //FT6206.touch_01_event = ((raw_tp_data[3]) >> 4) & 0b11;	//0b00->PressDown 0b01->LiftUp

  FT6206.touch_01_ypos = ((raw_tp_data[5]) & 0x0F);	// Mask low nibble of byte
  FT6206.touch_01_ypos <<= 8;					// left shift 8
  FT6206.touch_01_ypos |= raw_tp_data[6];		// OR with reg(0x06)

  //FT6206.touch_01_id = ((raw_tp_data[5]) >> 4) & 0b11;	//Invalid when ID 0x0F
	//FT6206.touch_01_weight = raw_tp_data[7];
	//FT6206.touch_01_area = raw_tp_data[8];

	/*	Can be used later if multiTouch is desired */
  //FT6206.touch_02_xpos = raw_tp_data[9] & 0x0F;	// Mask byte
  //FT6206.touch_02_xpos <<= 8;								// left shift 8
  //FT6206.touch_02_xpos |= raw_tp_data[10];			// OR with reg(0x04)

  //FT6206.touch_02_ypos = raw_tp_data[11] & 0x0F;	// Mask byte
  //FT6206.touch_02_ypos <<= 8;								// left shift 8
  //FT6206.touch_02_ypos |= raw_tp_data[12];			// OR with reg(0x04)

  //FT6206.touch_02_id |= (raw_tp_data[11] >> 4);
 	//FT6206.touch_02_weight = raw_tp_data[13];
 	//FT6206.touch_02_area = raw_tp_data[14];
}

