/**
  ******************************************************************************
  * @file    ili_9341.c 
  * @author  Multicalor NV
  * @version V1.0.0
  * @date    23-01-2015
  * @brief   Display Driver
	@brief	Verventa 1.0.0
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
/* Includes ------------------------------------------------------------------*/
#include "ili_9341.h"


void set_tft_window(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {

	LCD_REG  = COLUMN_ADDRESS_SET;
	LCD_DATA = 0;
	LCD_DATA = x1;
	LCD_DATA = 0;
	LCD_DATA = x2;

	LCD_REG  = PAGE_ADDRESS_SET;
	LCD_DATA = (y1 >> 8);
	LCD_DATA = (y1 & 0xFF);
	LCD_DATA = (y2 >> 8);
	LCD_DATA = (y2 & 0xFF);

}

void tft_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
	uint32_t pixels;
	pixels = w * h;
	set_tft_window(x, y, (x + w - 1), (y + h - 1));
	LCD_REG = MEMORY_WRITE;
	while (pixels--)
		LCD_DATA = color;
}

void tft_write_cmd (uint8_t cmd, uint16_t value) {
	LCD_REG = cmd;
	LCD_DATA = value;
}

uint16_t tft_read_reg(uint8_t reg){
	LCD_REG = reg;
	return LCD_DATA;
}

void tft_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
	if ((x>TFT_WIDTH) || (y>TFT_HEIGHT)) {
		return;
	}
	set_tft_window(x, y, TFT_WIDTH, TFT_HEIGHT);
	LCD_REG = MEMORY_WRITE;
	LCD_DATA = color;
}


void init_ili9341(void) {

	LCD_REG = 0x11;
	tft_delay(50000);

 	LCD_REG  = 0xCF;
	LCD_DATA = 0x00;
	LCD_DATA = 0xc3;
	LCD_DATA = 0X30;

 	LCD_REG  = 0xED;
	LCD_DATA = 0x64;
	LCD_DATA = 0x03;
	LCD_DATA = 0X12;
	LCD_DATA = 0X81;

 	LCD_REG  = 0xE8;
	LCD_DATA = 0x85;
	LCD_DATA = 0x10;
	LCD_DATA = 0x79;

 	LCD_REG  = 0xCB;
	LCD_DATA = 0x39;
	LCD_DATA = 0x2C;
	LCD_DATA = 0x00;
	LCD_DATA = 0x34;
	LCD_DATA = 0x02;

 	LCD_REG  = 0xF7;
	LCD_DATA = 0x20;

 	LCD_REG  = 0xEA;
	LCD_DATA = 0x00;
	LCD_DATA = 0x00;

 	LCD_REG  = 0xC0;    //Power control
	LCD_DATA = 0x22;   //VRH[5:0]

 	LCD_REG  = 0xC1;    //Power control
	LCD_DATA = 0x11;   //SAP[2:0];BT[3:0]

 	LCD_REG  = 0xC5;    //VCM control
	LCD_DATA = 0x3d;
    //LCD_DataWrite_ILI9341(0x30);
	LCD_DATA = 0x20;

 	LCD_REG = 0xC7;    //VCM control2
    //LCD_DataWrite_ILI9341(0xBD);
	LCD_DATA = 0xAA; //0xB0

 	LCD_REG  = 0x36;    // Memory Access Control
	LCD_DATA = 0x88;

 	LCD_REG  = 0x3A;
	LCD_DATA = 0x55;

 	LCD_REG  = 0xB1;
	LCD_DATA = 0x00;
	LCD_DATA = 0x13;

 	LCD_REG  = 0xB6;    // Display Function Control
	LCD_DATA = 0x0A;
	LCD_DATA = 0xA2;

 	LCD_REG  = 0xF6;
	LCD_DATA = 0x01;
	LCD_DATA = 0x30;

 	LCD_REG  = 0xF2;    // 3Gamma Function Disable
	LCD_DATA = 0x00;

 	LCD_REG  = 0x26;    //Gamma curve selected
	LCD_DATA = 0x01;

 	LCD_REG  = 0xE0;    //Set Gamma
	LCD_DATA = 0x0F;
	LCD_DATA = 0x3F;
	LCD_DATA = 0x2F;
	LCD_DATA = 0x0C;
	LCD_DATA = 0x10;
	LCD_DATA = 0x0A;
	LCD_DATA = 0x53;
	LCD_DATA = 0XD5;
	LCD_DATA = 0x40;
	LCD_DATA = 0x0A;
	LCD_DATA = 0x13;
	LCD_DATA = 0x03;
	LCD_DATA = 0x08;
	LCD_DATA = 0x03;
	LCD_DATA = 0x00;

 	LCD_REG  = 0XE1;    //Set Gamma
	LCD_DATA = 0x00;
	LCD_DATA = 0x00;
	LCD_DATA = 0x10;
	LCD_DATA = 0x03;
	LCD_DATA = 0x0F;
	LCD_DATA = 0x05;
	LCD_DATA = 0x2C;
	LCD_DATA = 0xA2;
	LCD_DATA = 0x3F;
	LCD_DATA = 0x05;
	LCD_DATA = 0x0E;
	LCD_DATA = 0x0C;
	LCD_DATA = 0x37;
	LCD_DATA = 0x3C;
	LCD_DATA = 0x0F;

	LCD_REG  = 0x11;    //Exit Sleep
	tft_delay(50000);
 	LCD_REG  = 0x29;    //Display on
	tft_delay(50000);


	/* INIT FOR NHD-2.4-240320SF-CTXI-F1
	// from NHD-2.4-240320SF-CTXI-F1.pdf
	LCD_REG = DISPLAY_OFF;
	LCD_REG = EXIT_SLEEP_MODE;
	LCD_DATA = 0x00;
	tft_delay(50000);

	LCD_REG = POWER_CONTROL_A;
	LCD_DATA = 0x39; //always 0x39
	LCD_DATA = 0x2C; //always 0x2C
	LCD_DATA = 0x00; //always 0x00
	LCD_DATA = 0x34; //Vcore = 1.6V
	LCD_DATA = 0x02; //DDVDH = 5.6V
	tft_delay(50000);

	LCD_REG = POWER_CONTROL_B;
	LCD_DATA = 0x00; //always 0x00
	LCD_DATA = 0x81; //PCEQ off
	LCD_DATA = 0x30; //ESD protection
	tft_delay(50000);

	LCD_REG = DRIVER_TIMING_CTRL_A; //Driver timing control A
	LCD_DATA = 0x85; //non-overlap
	LCD_DATA = 0x01; //EQ timing
	LCD_DATA = 0x79; //Pre-charge timing
	tft_delay(50000);

	LCD_REG = DRIVER_TIMING_CTRL_B; //Driver timing control B
	LCD_DATA = 0x00; //Gate driver timing
	LCD_DATA = 0x00; //always 0x00
	tft_delay(50000);

	LCD_REG = POWER_ON_SEQUENCE_CONTROL; //Power-On sequence control
	LCD_DATA = 0x64; //soft start
	LCD_DATA = 0x03; //power on sequence
	LCD_DATA = 0x12; //power on sequence
	LCD_DATA = 0x81; //DDVDH enhance on
	tft_delay(50000);

	LCD_REG = PUMP_RATIO_CONTROL; //Pump ratio control
	LCD_DATA = 0x20; //DDVDH=2xVCI
	tft_delay(50000);

	LCD_REG = POWER_CONTROL_1; //power control 1
	LCD_DATA = 0x26;
	LCD_DATA = 0x04; //second parameter for ILI9340 (ignored by ILI9341)
	tft_delay(50000);

	LCD_REG = POWER_CONTROL_2; //power control 2
	LCD_DATA = 0x11;
	tft_delay(50000);

	LCD_REG = VCOM_CONTROL_1; //VCOM control 1
	LCD_DATA = 0x35;
	LCD_DATA = 0x3E;
	tft_delay(50000);

	LCD_REG = VCOM_CONTROL_2; //VCOM control 2
	LCD_DATA = 0xBE;
	tft_delay(50000);

	// MADCTL
	LCD_REG = MEMORY_ACCESS_CONTROL; //memory access control = BGR
	*/
	/*
	 * 36h MADCTL (Memory Access Control)
	 * D7 D6 D5 D4  D3 D2 D1 D0 HEX
	 *  0  0  1  1   0  1  1  0 36h
	 * MY MX MV ML BGR MH  0  0 00
	 * MY -> Row address order
	 * MX -> Column address order
	 * MV -> Row/Column exchange
	 *
	 * LCD_DATA = 0x88;
	 *  +------+
	 *  |      |
	 *  |      |
	 * 0+------+
	 *  0|TAIL|
	 * Linksonder naar boven
	 *
	 * LCD_DATA = 0x48;
	 *         0
	 *  +------+0
	 *  |      |
	 *  |      |
	 *  +------+
	 *   |TAIL|
	 * Rechts boven naar beneden
	 *
	 * LCD_DATA = 0x08;
	 *  0
	 * 0+------+
	 *  |      |
	 *  |      |
	 *  +------+
	 *   |TAIL|
	 *  Links boven naar beneden
	 */
	/*
	LCD_DATA = ILI9341_MADCTL_BGR | ILI9341_MADCTL_ML; // BGR order

	tft_delay(50000);

	LCD_REG = FRAME_RATE_CONTROL; //frame rate control
	LCD_DATA = 0x00;
	LCD_DATA = 0x10;
	tft_delay(50000);

	// adafruit based
	LCD_REG = ENTRY_MODE_SET;
	LCD_DATA = 0x07;
	// end adafruit added
	tft_delay(50000);

	LCD_REG = DISPLAY_FUNCTION_CONTROL; //display function control
	LCD_DATA = 0x0A;
	LCD_DATA = 0xA2;
	tft_delay(50000);

	LCD_REG = PIXEL_FORMAT_SET; //pixel format = 16 bit per pixel
	LCD_DATA = 0x55;
	tft_delay(50000);

	LCD_REG = 0x00F2; //3G Gamma control
	LCD_DATA = 0x02; //off
	tft_delay(50000);

	LCD_REG = GAMMA_SET; //Gamma curve 3
	LCD_DATA = 0x01;
	tft_delay(50000);

	LCD_REG = COLUMN_ADDRESS_SET; //column address set
	LCD_DATA = 0x00;
	LCD_DATA = 0x00; //start 0x0000
	LCD_DATA = 0x00;
	LCD_DATA = 0xEF; //end 0x00EF = 239
	tft_delay(50000);

	LCD_REG = PAGE_ADDRESS_SET; //page address set
	LCD_DATA = 0x00;
	LCD_DATA = 0x00; //start 0x0000
	LCD_DATA = 0x01;
	LCD_DATA = 0x3F; //end 0x013F 319
	tft_delay(50000);

	LCD_REG = DISPLAY_ON;

	tft_delay(50000);

	LCD_REG = MEMORY_WRITE;
	*/


}

/*void init_tft_io(void) {
	 // TODO -> PUT RESET ON GPIO PIN SO WE CAN RESET DISPLAY
	 // now reset is on NRST pin on the STM32F4 discovery 
	 GPIO_InitTypeDef GPIO_InitStructure;
  // Clock Enable Port-D and Port-E
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);
	// PORTD setup -> alternate function
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0,  GPIO_AF_FSMC); // PD0=FSMC_D2   -> DB2
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1,  GPIO_AF_FSMC); // PD1=FSMC_D3   -> DB3
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4,  GPIO_AF_FSMC); // PD4=FSMC_NOE  -> RD
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5,  GPIO_AF_FSMC); // PD5=FSMC_NWE  -> WR
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource7,  GPIO_AF_FSMC); // PD7=FSMC_NE1  -> CS
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8,  GPIO_AF_FSMC); // PD8=FSMC_D13  -> DB15
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9,  GPIO_AF_FSMC); // PD9=FSMC_D14  -> DB16
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC); // PD10=FSMC_D15 -> DB17
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC); // PD11=FSMC_A16 -> RS
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC); // PD14=FSMC_D0  -> DB0
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC); // PD15=FSMC_D1  -> DB1
  // Set up PORT D
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0	| GPIO_Pin_1 	| GPIO_Pin_4	|
  															GPIO_Pin_5 	| GPIO_Pin_7	| GPIO_Pin_8 	|
  															GPIO_Pin_9 	| GPIO_Pin_10 | GPIO_Pin_11 |
  															GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  // Write port D
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	// PORTE setup -> alternate function
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7,  GPIO_AF_FSMC); // PE7=FSMC_D4   -> DB4
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8,  GPIO_AF_FSMC); // PE8=FSMC_D5   -> DB5
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9,  GPIO_AF_FSMC); // PE9=FSMC_D6   -> DB6
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC); // PE10=FSMC_D7  -> DB7
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC); // PE11=FSMC_D8  -> DB10
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC); // PE12=FSMC_D9  -> DB11
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC); // PE13=FSMC_D10 -> DB12
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC); // PE14=FSMC_D11 -> DB13
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC); // PE15=FSMC_D12 -> DB14
  // Set up PORT E
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 	| GPIO_Pin_8 	| GPIO_Pin_9 	|
  															GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |
  															GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	// Write PORT E
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}*/

/*void init_tft_FSMC(void) {
  FSMC_NORSRAMInitTypeDef        FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMTimingInitStructure;
  // Clock Enable to FSMC
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
  // Timing structure
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime 			= LCD_ILI9341_FSMC_AST;
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime 				= 0x0000;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime 					= LCD_ILI9341_FSMC_DST;
  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration 	= 0x0000;
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision 						= 0x0000;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency 						= 0x0000;
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode 						= FSMC_AccessMode_A;

  // Bank-1 / PSRAM-1
  FSMC_NORSRAMInitStructure.FSMC_Bank 												= FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux 							= FSMC_DataAddressMux_Disable;
  // FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType 									= FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth 							= FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode 							= FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait 						= FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity 					= FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode 										= FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive 						= FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation 							= FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal 									= FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode 								= FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst 									= FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct 				= &FSMC_NORSRAMTimingInitStructure;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct 						= &FSMC_NORSRAMTimingInitStructure;
  // FSMC config
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
  // Enable Bank-1 / PSRAM-1
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}*/

void tft_delay (volatile uint32_t count){
	while (count--) {
	}
}
