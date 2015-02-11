/**
  ******************************************************************************
  * @file    ili_9341.h 
  * @author  Multicalor
  * @version V1.0.0
  * @date    23-01-2015
  * @brief   Header for ili_9341.c module
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
#ifndef __ILI_9341_H
#define __ILI_9341_H
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* defines ------------------------------------------------------------*/
// ILI9341 Command list
#define	NOP 										0x00
#define	SOFT_RESET 									0x01
#define	READ_DISPLAY_ID								0x04
#define	READ_DISPLAY_STATUS							0x09
#define	READ_DISPLAY_POWER_MODE						0x0A
#define	READ_DISPLAY_MADCTL							0x0B
#define	READ_DISPLAY_PIXEL_FORMAT					0x0C
#define	READ_DISPLAY_IMAGE_MODE						0x0D
#define	READ_DISPLAY_SIGNAL_MODE					0x0E
#define	READ_DISPLAY_SELF_DIAGNOSTICS				0x0F
#define	ENTER_SLEEP_MODE							0x10
#define	EXIT_SLEEP_MODE								0x11
#define	PARTIAL_MODE_ON								0x12
#define	NORMAL_DISPLAY_MODE_ON						0x13
#define	DISPLAY_INVERSION_OFF						0x20
#define	DISPLAY_INVERSION_ON						0x21
#define	GAMMA_SET									0x26
#define	DISPLAY_OFF									0x28
#define	DISPLAY_ON									0x29
#define	COLUMN_ADDRESS_SET							0x2A
#define	PAGE_ADDRESS_SET							0x2B
#define	MEMORY_WRITE								0x2C
#define	COLOUR_SET									0x2D
#define	MEMORY_READ									0x2E
#define	PARTIAL_AREA								0x30
#define	VERTICAL_SCROLLING_DEFINITION				0x33
#define	TEARING_EFFECT_LINE_OFF						0x34
#define	TEARING_EFFECT_LINE_ON						0x35
#define	MEMORY_ACCESS_CONTROL						0x36
#define	VERTICAL_SCROLLING_START_ADDRESS			0x37
#define	IDLE_MODE_OFF								0x38
#define	IDLE_MODE_ON								0x39
#define	PIXEL_FORMAT_SET							0x3A
#define	WRITE_MEMORY_CONTINUE						0x3C
#define	READ_MEMORY_CONTINUE						0x3E
#define	SET_TEAR_SCANLINE							0x44
#define GET_SCANLINE								0x45
#define	WRITE_DISPLAY_BRIGHTNESS					0x51
#define	READ_DISPLAY_BRIGHTNESS						0x52
#define	WRITE_CONTROL_DISPLAY						0x53
#define	READ_CONTROL_DISPLAY						0x54
#define	WRITE_CABC									0x55
#define	READ_CABC 									0x56
#define	WRITE_MIN_CABC								0x5E
#define	READ_MIN_CABC 								0x5F
#define FRAME_RATE_CONTROL							0xB1
#define DISPLAY_FUNCTION_CONTROL					0xB6
#define ENTRY_MODE_SET								0xB7
#define POWER_CONTROL_1								0xC0
#define POWER_CONTROL_2								0xC1
#define VCOM_CONTROL_1								0xC5
#define VCOM_CONTROL_2								0xC7
#define POWER_CONTROL_A								0xCB
#define POWER_CONTROL_B								0xCF
#define	READ_ID1									0xDA
#define	READ_ID2									0xDB
#define	READ_ID3									0xDC
#define DRIVER_TIMING_CTRL_A						0xE8
#define DRIVER_TIMING_CTRL_B						0xEA
#define POWER_ON_SEQUENCE_CONTROL					0xED
#define PUMP_RATIO_CONTROL							0xF7


#define ILI9341_MADCTL_MY  				0x80		// column address order
#define ILI9341_MADCTL_MX  				0x40		// row address order
#define ILI9341_MADCTL_MV  				0x20		// vertical refresh order
#define ILI9341_MADCTL_ML  				0x10		// vertical refresh order
#define ILI9341_MADCTL_RGB 				0x00		// red green blue
#define ILI9341_MADCTL_BGR 				0x08		// blue green red
#define ILI9341_MADCTL_MH  				0x04		// horizontal refresh order

#define PORTRAIT								0
#define LANDSCAPE								1
#define PORTRAIT_180							2
#define LANDSCAPE_270							3

/* FSMC CONNECTION OF ILI 9341 ON STM32F4 DISCOVERY
 * D0			|	PD14		|	BROWN
 * D1			|	PD15		|	RED
 * D2			|	PD0			|	ORANGE
 * D3			|	PD1			|	YELLOW
 * D4			|	PE7			|	GREEN
 * D5			|	PE8			|	BLUE
 * D6			|	PE9			|	PURPLE
 * D7			|	PE10		|	GREY
 * D8			| 	PE11		|	BROWN
 * D9			|	PE12		|	RED
 * D10			|	PE13		| 	ORANGE
 * D11			|	PE14		| 	YELLOW
 * D12			|	PE15		|	GREEN
 * D13			|	PD8			|	BLUE
 * D14			|	PD9			|	PURPLE
 * D15			|	PD10		| 	GREY
 * CS			|	PD7			| 	BLACK
 * RS			|	PD11		| 	PURPLE
 * RD			|	PD4			|	GREY
 * WR			|	PD4			|	WIT
 * RESET 		|	VDD			| 	YELLOW
 * TODO -> PUT RESET ON GPIO PIN SO WE CAN RESET DISPLAY
 */

/* Standard colors 16 bit R5G6B5
 * Red 		|	5 bit		|	Bit 15-11
 * Green	|	6 bit		|	Bit 10- 5
 * Blue 	|	5 bit		|	Bit  4- 0
 */
#define  RGB_BLACK          0x0000
#define  RGB_WHITE          0xFFFF
#define  RGB_GREY						0x8410
#define  RGB_SILVER					0xC618
#define  RGB_RED            0xF800
#define  RGB_YELLOW         0xFFE0
#define  RGB_GREEN					0x0400
#define  RGB_BLUE           0x001F
#define  RGB_MAROON         0x8000
#define  RGB_OLIVE					0x8400
#define  RGB_NAVY						0x0010
#define  RGB_PURPLE					0x8010
#define  RGB_TEAL						0x0410
#define  RGB_LIME	          0x07E0
#define  RGB_LIGHTPINK			0xFDB8
#define  RGB_CORAL					0xFBEA
#define  RGB_MONEYGREEN			0xC6F8
#define  RGB_SKYBLUE				0xA65E
#define  RGB_CREAM					0xFFDE
#define  RGB_CYAN           0x07FF
#define  RGB_MAGENTA        0xF81F
#define  RGB_BROWN        	0xA145
#define  RGB_PLUM	        	0xDD1B
#define  RGB_LIGHTGREY     	0xD69A
#define  RGB_BURLYWOOD     	0xDDD0

//--------------------------------------------------------------
// Map FSMC address to display
// Bank 1 0x60000000
// RS-Pin = PD11 = FSMC_A16 = Offset  0x00200000
// Page 1316/1317 from Reference Manual
//--------------------------------------------------------------
#define LCD_REG  (*((volatile unsigned short *) 0x60000000)) // RS = 0 REGISTER !!
#define LCD_DATA (*((volatile unsigned short *) 0x60020000)) // RS = 1 DATA     !!
#define LCD_DATA_ADR   0x60020000

#define  LCD_ILI9341_FSMC_AST  	0x0002 // AdressSetupTime
#define  LCD_ILI9341_FSMC_DST  	0x0005 // DataSetupTime

#define TFT_WIDTH		239
#define TFT_HEIGHT		319

/* Exported functions ------------------------------------------------------- */
void init_tft_io(void);
void init_tft_FSMC(void);
void init_ili9341(void);

void set_tft_window(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
uint16_t tft_read_reg(uint8_t reg);
void tft_write_cmd (uint8_t cmd, uint16_t value);
void tft_delay (volatile uint32_t count);
void tft_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void tft_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

//void     setRotation(uint8_t x);
// extern void tft_soft_reset(void);


// void     drawFastHLine(int16_t x0, int16_t y0, int16_t w, uint16_t color);
// void     drawFastVLine(int16_t x0, int16_t y0, int16_t h, uint16_t color);
// void     fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c);
// void     fillScreen(uint16_t color);









#endif
