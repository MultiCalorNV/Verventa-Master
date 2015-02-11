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
  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FT6206_H
#define __FT6206_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* defines ------------------------------------------------------------*/
#define FT6206_ADDR          								0x38 	// 0x38 << 1
#define FT6206_REG_NUMTOUCHES 								0x02

#define FT6206_NUM_X             							0x33
#define FT6206_NUM_Y             							0x34

#define FT6206_REG_MODE 									0x00
#define FT6206_REG_CALIBRATE 								0x02
#define FT6206_REG_WORKMODE 								0x00
#define FT6206_REG_FACTORYMODE 								0x40
#define FT6206_REG_THRESHOLD 								0x80
#define FT6206_REG_POINTRATE 								0x88
#define FT6206_REG_FIRMVERS 								0xA6
#define FT6206_REG_CHIPID 									0xA3
#define FT6206_REG_VENDID 									0xA8
#define	FT6206_REG_G_MODE									0xA4

// gesture id's
#define FT6206_MOVE_UP										0x10
#define FT6206_MOVE_RIGHT									0x14
#define FT6206_MOVE_DOWN									0x18
#define FT6206_MOVE_LEFT									0x1C
#define FT6206_ZOOM_IN										0x48
#define FT6206_ZOOM_OUT										0x49
#define FT6206_NO_GESTURE									0x00

// events
#define FT6206_PRESS_DOWN									0b00
#define FT6206_LIFT_UP										0b01
#define FT6206_CONTACT										0b10
#define FT6206_NO_EVENT										0b11

// Interrupt Mode
#define FT6206_INT_POLLING_MODE								0x00
#define	FT6206_INT_TRIGGER_MODE								0x01 


// calibrated for Adafruit 2.8" ctp screen
#define FT6206_DEFAULT_THRESHOLD 128

/* Exported constants --------------------------------------------------------*/
typedef struct FT6206_t{
	uint8_t gesture_id;		// gesture id
	uint8_t touch_points;	// # of touch points
	uint8_t	 touch_01_event;
	uint16_t touch_01_xpos;
	uint8_t	 touch_01_id;	// 0x0F when invalid
	uint16_t touch_01_ypos;
	uint8_t	 touch_01_weight;
	uint8_t	 touch_01_area;
	uint8_t	 touch_02_event;
	uint16_t touch_02_xpos;
	uint8_t	 touch_02_id;	// 0x0F when invalid
	uint16_t touch_02_ypos;
	uint8_t	 touch_02_weight;
	uint8_t	 touch_02_area;
	//
	uint8_t vendor_id;
	uint8_t chip_id;
	uint8_t  firmware_version;
	uint8_t pointrate;
	uint8_t	 threshold;
} FT6206_t;

extern FT6206_t FT6206;

typedef enum Event{
	Press_Down	= 0,
	Lift_Up		= 1,
	Contact		= 2,
	No_Event	= 3
} Event_Flag;
	
/* Exported functions ------------------------------------------------------- */
uint8_t FT6206_begin(uint8_t threshhold);
void FT6206_read_data(void);
uint8_t FT6206_touched(void);

#ifdef __cplusplus
}
#endif

#endif
