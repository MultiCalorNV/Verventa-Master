/*********************************************************************
*          					MULTICALOR NV			                 *
*          												             *
*        													         *
**********************************************************************
*                                                                    *
*        (c) 2014  MULTICALOR Eng.							         *
*                                                                    *
*        														     *
*                                                                    *
**********************************************************************

** V1.0.0														    **
All  Intellectual Property rights  in the Software belongs to  
MULTICALOR.
Knowledge of thesource code may not be used to write a similar product.  

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_verventa.h
Purpose     : Sets up a multi Window GUI
----------------------------------------------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
#ifndef __GUI_verventa_H
#define __GUI_verventa_H
/*	includes-----------------------------------------------------*/
#include <stdbool.h>
#include "main.h"
#include "DIALOG.h"

/*	Exported Variables-------------------------------------------*/
extern bool Touched_Flag;
extern bool Touched_Button2;
extern bool Touch_thermo;

extern WM_HWIN _hWindow4;
extern WM_HWIN _hWindow5;
extern WM_HWIN _hWindow6;

extern WM_HWIN hWinThermostat;
extern WM_HWIN hWinSetup;
extern WM_HWIN hWinSetTime;
extern WM_HWIN hWinProgramTimes;
extern WM_HWIN hWinProgramTime;

extern WM_HWIN hWinFanImg;

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif
extern GUI_CONST_STORAGE GUI_BITMAP bmfan_Rot0;
extern GUI_CONST_STORAGE GUI_BITMAP bmfan_Rot1;
extern GUI_CONST_STORAGE GUI_BITMAP bmfan_Rot2;
extern GUI_CONST_STORAGE GUI_BITMAP bmfan_Rot3;
extern GUI_CONST_STORAGE GUI_BITMAP bmfan_Stop0;


#define TEMP_MIN (65)
#define TEMP_MAX (95)
#define SHOW_FAN

#define NUM_PROGRAM_TIMES (6)

//extern PROGRAMTIME g_pgmAry[NUM_PROGRAM_TIMES];
extern int g_curPgmIdx;
extern int g_manSetTmp;
extern int g_hour;
extern int g_isPM;
extern int g_min;
extern int g_dow;

extern bool g_secTgl;
//extern DS_ManualCtl g_manCtl;
//extern const PROGRAMTIME *g_pEffPgm;

/**********************************************
*	Public Functions
***********************************************
*/
ErrorStatus Init_GUI_verventa(void);
void AGUI_UpdateCurTemperature(signed int f);
void AGUI_ShowRTC(void);
WM_HWIN CreateThermostat(void);
void ShowWindow(WM_HWIN window);
void UpdateTimeDisplay(void);

/**********************************************
*	TypeDefines
***********************************************
*/
typedef enum _enum_AirCtl
{
	airctl_auto = 0,
	airctl_cooler,
	airctl_heater,
	airctl_off,
}enm_AirCtl;

typedef enum _enum_FanCtl
{
	fanctl_auto = 0,
	fanctl_on,
	fanctl_off,
}enm_FanCtl;


typedef struct _DS_ManualCtl
{
	bool isEn;
	uint8_t airCtl;	// 0=auto, 1=
	uint8_t fanCtl;
	uint8_t pad;
	int32_t cx16setTmp;
}DS_ManualCtl;

// this structure must be 32 byte size!
typedef struct programTime
{
	int hour;	// 1-12 = hour; -1 = Free (used to load settings from eeprom)
	int minute;
	int ampm;	// 0 = am, 1 = pm
	int temp;
	int dayOfWeekMask; // Sun = 1<<6, Mon = 1<<5, Tue = 1<<4, Wed = 1<<3, Thu = 1<<2, Fri = 1<<1, Sat = 1<<0
}PROGRAMTIME;

// Record temperature every 5 seconds for most recent
#define TMP_REC_ITVL	5
#define TMP_REC_PTS	(3600/5) * 6

typedef struct _DS_TmpRec
{
	int16_t cx16Ary[TMP_REC_PTS];
	uint32_t wrIdx;
	uint32_t cnt;
	uint32_t prscVal;	// prescaler value, auxiliary
}DS_TmpRec;

typedef struct _DS_DateTime
{
	int a;
}DS_DateTime;

#endif  /* Avoid multiple inclusion */