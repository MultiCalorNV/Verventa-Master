/**
  ******************************************************************************
  * @file    stm32_ub_stemwin.c
  * @author  Multicalor
  * @version V1.0.0
  * @date    23-01-2015
  * @brief   STemWin GUI init + Touch init
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
#include "stm32_ub_stemwin.h"
#include "stm32f4xx_it.h"

/**
  * @brief  This function Init STemWin Display Touchpanel.
  * @param  None
  * @retval ErrorStatus
  */
ErrorStatus UB_STemWIN_init(void){
  int16_t error_stat;
  uint8_t threshhold = 0;
  RCC_ClocksTypeDef RCC_Clocks;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE); // Don't forget!!
  
  /*	Display Init	*/
  init_ili9341();
  HAL_Delay(2);

  /*	Touch Init	*/
  FT6206_begin(threshhold);
  HAL_Delay(5);

  /*	STemWin-GUI Init	*/
  if(GUI_Init()!=0) {
    return(ERROR);
  }
  
  return(SUCCESS);
}

 

/**
  * @brief  This function updates Touch every 3ms.
  * @param  None
  * @retval None
  */
void UB_STemWIN_updateTouch(void){
  GUI_PID_STATE TS_State;
  static uint8_t prev_state = 0;
  uint32_t TS_Orientation = 1;  // LandScape Mode
  
  /*	Call Touches	*/
  FT6206_read_data();

    if(TS_Orientation == 0){
		TS_State.x = FT6206.touch_01_xpos;
		TS_State.y = FT6206.touch_01_ypos;
	}
	else{
		TS_State.y = 240 - FT6206.touch_01_xpos;
		TS_State.x = FT6206.touch_01_ypos;
	}
	
	if((FT6206.touch_points) != 0){
		TS_State.Pressed = 1;
	}
	else{
		TS_State.Pressed = 0;
	}

	if(prev_state != TS_State.Pressed ){
		prev_state = TS_State.Pressed;
		TS_State.Layer = 0;
		GUI_TOUCH_StoreStateEx(&TS_State);
	}
	
}


