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
File        : GUI_verventa.c
Purpose     : Sets up a multi Window GUI + Buttons
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
//------------------------------------
// Includes
//------------------------------------
#include <string.h>
#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "PROGBAR.h"
#include "GUI_verventa.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
#define	SPEED	1250
#define	MSG_CHANGE_TEXT	(WM_USER + 0)
#define	ID_BUTTON_0		(GUI_ID_USER + 0x0B)
#define	ID_BUTTON_1		(GUI_ID_USER + 0x0C)
#define	ID_BUTTON_2		(GUI_ID_USER + 0x0D)
#define	ID_BUTTON_3		(GUI_ID_USER + 0x0E)
#define	ID_BUTTON_4		(GUI_ID_USER + 0x0F)
#define	ID_BUTTON_5		(GUI_ID_USER + 0x10)
#define	ID_BUTTON_6		(GUI_ID_USER + 0x11)
#define	ID_BUTTON_7		(GUI_ID_USER + 0x12)
#define	ID_BUTTON_8		(GUI_ID_USER + 0x13)
#define	ID_PROGBAR_1	(GUI_ID_USER + 0x14)
#define	ID_SLIDER_1		(GUI_ID_USER + 0x15)

/*********************************************************************
*	Static vars
**********************************************************************
*/
static char _acInfoText[40];
static WM_CALLBACK *_cbBkWindowOld;
WM_CALLBACK *cbButton0;
WM_CALLBACK *cbButton1;
WM_CALLBACK *cbButton2;
WM_CALLBACK *cbButton3;
WM_CALLBACK *cbButton4;
/*** Handles ***/
WM_HWIN _hWindow4;
WM_HWIN _hWindow5;
WM_HWIN _hWindow6;
static WM_HWIN _hChild;
static BUTTON_Handle _hButton_button1;
static BUTTON_Handle _hButton_button2;
static BUTTON_Handle _hButton_button3;
static BUTTON_Handle _hButton_button4;
static BUTTON_Handle _hButton_button5;
static BUTTON_Handle _hButton_button6;
static BUTTON_Handle _hButton_button7;
static BUTTON_Handle _hButton_button8;
static BUTTON_Handle _hButton_button9;
static PROGBAR_Handle _hProgBar_1;
static SLIDER_Handle _hSlider_1;
static int Id_ok = GUI_ID_OK;
static int Id_clear = GUI_ID_TEXT0;


/*********************************************************************
*	Flags
**********************************************************************
*/
bool Touched_Flag = false;
bool Touched_Button2 = false;
//int Id;


/*********************************************************************
*
*       Private Functions
*
**********************************************************************
*/
static void _SsetDesktopColor(void);
static void _DrawInfoText(char *pStr);
static void _cbBkWindow(WM_MESSAGE *pMsg);
static void _cbWindow1(WM_MESSAGE *pMsg);
static void _cbWindow2(WM_MESSAGE *pMsg);
static void _cbChild(WM_MESSAGE *pMsg);
static void _cbButton(WM_MESSAGE *pMsg);
static void _CreateWindow(void);
static void _CreateWindowAsChild(void);
static void _BringToTop(WM_HWIN _hWin);
static void _BringToBottom(WM_HWIN _hWin);
static void _ProgBarStat(PROGBAR_Handle _hProgBar);

/*********************************************************************
*	BUTTON_Callback()
*		:Receive messages from button
**********************************************************************
*/
static void _cbButton(WM_MESSAGE *pMsg){
	int Id, NCode;
	GUI_PID_STATE current_PIDState;
	
	switch(pMsg->MsgId){
		/*case WM_TOUCH:
			WM_InvalidateWindow(pMsg->hWin);
			GUI_PID_GetCurrentState(&current_PIDState);
			if(current_PIDState.Pressed == 1){
				Id = WM_GetId(pMsg->hWin);
				switch(Id){
					case ID_BUTTON_0:
						//_BringToBottom(_hWindow4);
						ShowWindow(_hWindow5);
						break;
					case ID_BUTTON_3:
						//_BringToBottom(_hWindow4);
						ShowWindow(hWinThermostat);
						break;
					case ID_BUTTON_6:
						PROGBAR_SetValue(_hProgBar_1, 0);
						//_BringToBottom(_hWindow5);
						ShowWindow(_hWindow4);
						break;
					case ID_BUTTON_7:
						_ProgBarStat(_hProgBar_1);
						break;
					case ID_BUTTON_8:
						//_BringToBottom(_hWindow6);
						ShowWindow(_hWindow4);
						break;
					default:
						break;
					}
				if(UnPressed_Flag == true){
				}
			}
			break;*/		
		default:
			BUTTON_Callback(pMsg);
			break;
	}
}

/*********************************************************************
*	_cbBkWindow
*		:CallBack for BkWindow
**********************************************************************
*/
static void _cbBkWindow(WM_MESSAGE *pMsg){
	switch(pMsg->MsgId){
	case WM_PAINT:
		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(&GUI_Font24_ASCII);
		GUI_DispStringAt(_acInfoText, 5, 40);
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

/*********************************************************************
*	_cbWindow1
*		:CallBack for BkWindow
**********************************************************************
*/
void _cbWindow1(WM_MESSAGE *pMsg){
	GUI_COLOR Color;
	int Id, NCode;
	switch(pMsg->MsgId){
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) {
				case ID_BUTTON_0: // Notifications sent by 'btnSetup'
					Touched_Button2 = true;
					switch(NCode) {
					  case WM_NOTIFICATION_CLICKED:
						//ShowWindow(_hWindow5);
						break;
					  case WM_NOTIFICATION_RELEASED:
						ShowWindow(_hWindow5);
						Touch_thermo = true;
						break;
					}
				break;
				case ID_BUTTON_3: // Notifications sent by 'btnSetup'
					switch(NCode) {
					  case WM_NOTIFICATION_CLICKED:
						break;
					  case WM_NOTIFICATION_RELEASED:
						ShowWindow(hWinThermostat);
						Touch_thermo = true;
						break;
					}
				break;
			}
			break;
		case WM_PAINT:
			WM_GetUserData(pMsg->hWin, &Color, 4);
			GUI_SetBkColor(Color);
			GUI_Clear();
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
		}
}

/*********************************************************************
*	_cbWindow2
*		:CallBack for BkWindow
**********************************************************************
*/
static void _cbWindow2(WM_MESSAGE *pMsg){
	GUI_COLOR Color;
	int Id, NCode;
	switch(pMsg->MsgId){
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) {
				case ID_BUTTON_6: // Notifications sent by 'btnSetup'
					switch(NCode) {
					  case WM_NOTIFICATION_CLICKED:
						break;
					  case WM_NOTIFICATION_RELEASED:
						ShowWindow(_hWindow4);
						PROGBAR_SetValue(_hProgBar_1, 0);
						Touch_thermo = true;
						break;
					}
				break;
				case ID_BUTTON_7: // Notifications sent by 'btnSetup'
					switch(NCode) {
					  case WM_NOTIFICATION_CLICKED:
						break;
					  case WM_NOTIFICATION_RELEASED:
						_ProgBarStat(_hProgBar_1);
						Touch_thermo = true;
						break;
					}
				break;
			}
			break;
		case WM_PAINT:
			WM_GetUserData(pMsg->hWin, &Color, 4);
			GUI_SetBkColor(Color);
			GUI_Clear();
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
		}
}

/*********************************************************************
*	_cbChild
*		:CallBack for BkWindow
**********************************************************************
*/
static void _cbChild(WM_MESSAGE *pMsg){
	GUI_COLOR Color;
	switch(pMsg->MsgId){
		case WM_TOUCH:
			break;
		case WM_PAINT:
			WM_GetUserData(pMsg->hWin, &Color, 4);
			GUI_SetBkColor(Color);
			GUI_Clear();
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
		}
}

/*********************************************************************
*	_CreateWindow
*		:Create all windows
**********************************************************************
*/
static void _CreateWindow(void){
	_cbBkWindowOld = WM_SetCallback(WM_HBKWIN, _cbBkWindow);
	
	_hWindow4 = WM_CreateWindow(0, 0, 320, 240, WM_CF_SHOW | WM_CF_MEMDEV, _cbWindow1, 0);
	//GUI_Delay(SPEED/3);
	_hWindow5 = WM_CreateWindow(0, 0, 320, 240, WM_CF_SHOW | WM_CF_MEMDEV, _cbWindow2, 0);
	//GUI_Delay(SPEED/3);
	_hWindow6 = WM_CreateWindow(0, 0, 320, 240, WM_CF_SHOW | WM_CF_MEMDEV, _cbWindow2, 0);
	//GUI_Delay(SPEED/3);
	
	ShowWindow(_hWindow4);
	
}

/*********************************************************************
*	_CreateWindowAsChild
*		:Create all child windows
**********************************************************************
*/
static void _CreateWindowAsChild(void){
	_cbBkWindowOld = WM_SetCallback(WM_HBKWIN, _cbBkWindow);
	
	_hButton_button1 = BUTTON_CreateAsChild(0, 0, 150, 50, _hWindow4, ID_BUTTON_0, WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFont(_hButton_button1, &GUI_Font8x16);
	BUTTON_SetText(_hButton_button1, "Menu C °");
	BUTTON_SetFocussable(_hButton_button1, 1);
	BUTTON_SetSkin(_hButton_button1, BUTTON_SKIN_FLEX);
	

	_hButton_button2 = BUTTON_CreateAsChild(0, 60, 150, 50, _hWindow4, ID_BUTTON_1, WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFont(_hButton_button2, &GUI_Font8x16);
	BUTTON_SetText(_hButton_button2, "Menu T °");
	BUTTON_SetFocussable(_hButton_button2, 1);
	BUTTON_SetSkin(_hButton_button2, BUTTON_SKIN_FLEX);
	

	_hButton_button3 = BUTTON_CreateAsChild(0, 120, 150, 50, _hWindow4, ID_BUTTON_2, WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFont(_hButton_button3, &GUI_Font8x16);
	BUTTON_SetText(_hButton_button3, "Menu Settings");
	BUTTON_SetFocussable(_hButton_button3, 1);
	BUTTON_SetSkin(_hButton_button3, BUTTON_SKIN_FLEX);
	

	_hButton_button4 = BUTTON_CreateAsChild(160, 0, 150, 50, _hWindow4, ID_BUTTON_3, WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFont(_hButton_button4, &GUI_Font8x16);
	BUTTON_SetText(_hButton_button4, "Menu Vent");
	BUTTON_SetFocussable(_hButton_button4, 1);
	BUTTON_SetSkin(_hButton_button4, BUTTON_SKIN_FLEX);
	

	_hButton_button5 = BUTTON_CreateAsChild(160, 60, 150, 50, _hWindow4, ID_BUTTON_4, WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFont(_hButton_button5, &GUI_Font8x16);
	BUTTON_SetText(_hButton_button5, "Touch Up");
	BUTTON_SetFocussable(_hButton_button5, 1);
	BUTTON_SetSkin(_hButton_button5, BUTTON_SKIN_FLEX);
	

	_hButton_button6 = BUTTON_CreateAsChild(160, 120, 150, 50, _hWindow4, ID_BUTTON_5, WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFont(_hButton_button6, &GUI_Font8x16);
	BUTTON_SetText(_hButton_button6, "Touch Down");
	BUTTON_SetFocussable(_hButton_button6, 1);
	BUTTON_SetSkin(_hButton_button6, BUTTON_SKIN_FLEX);
	

	
	_hButton_button7 = BUTTON_CreateAsChild(0, 50, 320, 50, _hWindow5, ID_BUTTON_6, WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFont(_hButton_button7, &GUI_Font24B_ASCII);
	BUTTON_SetText(_hButton_button7, "MAIN");
	BUTTON_SetFocussable(_hButton_button7, 1);
	

	_hButton_button8 = BUTTON_CreateAsChild(0, 110, 320, 50, _hWindow5, ID_BUTTON_7, WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFont(_hButton_button8, &GUI_Font24B_ASCII);
	BUTTON_SetText(_hButton_button8, "Load Temp");
	BUTTON_SetFocussable(_hButton_button8, 1);
	BUTTON_SetSkin(_hButton_button8, BUTTON_SKIN_FLEX);
	

	_hProgBar_1 = PROGBAR_CreateAsChild(20, 200, 200, 25, _hWindow5, ID_PROGBAR_1, WM_CF_SHOW | WM_CF_MEMDEV);
	//PROGBAR_SetMinMax(_hProgBar_1, -16383, 16383);
	//PROGBAR_SetText(_hProgBar_1, "Load in temp...");
	PROGBAR_SetSkin(_hProgBar_1, PROGBAR_SKIN_FLEX);
	//PROGBAR_SetValue(_hProgBar_1, 50);
	
	_hButton_button9 = BUTTON_CreateAsChild(0, 50, 320, 50, _hWindow6, ID_BUTTON_8, WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFont(_hButton_button9, &GUI_Font24B_ASCII);
	BUTTON_SetText(_hButton_button9, "MAIN");
	BUTTON_SetFocussable(_hButton_button9, 1);
	
	_hSlider_1 = SLIDER_CreateEx(20, 180, 150, 35, _hWindow4, WM_CF_SHOW | WM_CF_MEMDEV, 0, ID_SLIDER_1);
	
	GUI_Exec();
	

	
	cbButton0 = WM_SetCallback(_hButton_button1, _cbButton);
	cbButton3 = WM_SetCallback(_hButton_button4, _cbButton);
	cbButton1 = WM_SetCallback(_hButton_button7, _cbButton);
	cbButton2 = WM_SetCallback(_hButton_button8, _cbButton);
	cbButton4 = WM_SetCallback(_hButton_button9, _cbButton);
	
}

/*********************************************************************
*	_SetDesktopColor
*		:Set Color for desktop Black
**********************************************************************
*/
static void _SetDesktopColor(void){
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	_DrawInfoText("WM_SetDesktopColor()");
	GUI_Exec();
	//GUI_Delay(SPEED/2);
	WM_SetDesktopColor(GUI_BLACK);
	GUI_Exec();
	//GUI_Delay(SPEED/2);
	GUI_SetBkColor(GUI_BLACK);
	WM_SetDesktopColor(GUI_INVALID_COLOR);
	
}

/*********************************************************************
*	_DrawInfoText
*		:Drawes the info text directly on the display
**********************************************************************
*/
static void _DrawInfoText(char *pStr){
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_DispStringHCenterAt("WindowManager - MultiCalor", 160, 5);
	GUI_SetFont(&GUI_Font8x16);
	GUI_DispStringAtCEOL(pStr, 5, 40);
}

/*********************************************************************
*	_BringToTop
*		:Set a window as Top Window
***********************************************************************
*/
static void _BringToTop(WM_HWIN _hWin){
	WM_BringToTop(_hWin);
	GUI_Exec();
}

/*********************************************************************
*	_BringToBottom
*		:Set a window as bottom Window
***********************************************************************
*/
static void _BringToBottom(WM_HWIN _hWin){
	WM_BringToBottom(_hWin);
	GUI_Exec();
}

/*********************************************************************
*	_ProgBarStat
*		:Set progbar stat
***********************************************************************
*/
static void _ProgBarStat(PROGBAR_Handle _hProgBar){
	int n;
	
	//PROGBAR_SetValue(_hProgBar, -16383);
	for(n=0; n <= 100; n++){
		PROGBAR_SetValue(_hProgBar, n);
	}
}

/*********************************************************************
*	ShowWindow
*		:Hide all windows and then show 'Window'
**********************************************************************
*/
void ShowWindow(WM_HWIN window)
{
	WM_HideWindow(hWinThermostat);
	WM_HideWindow(_hWindow4);
	WM_HideWindow(_hWindow5);
	WM_HideWindow(_hWindow6);
	//WM_HideWindow(hWinProgramTime);
	WM_ShowWindow(window);
	GUI_Exec();
}

/*********************************************************************
*	Init_GUI_calor
*		:Init all windows and callbacks for the GUI
**********************************************************************
*/
ErrorStatus Init_GUI_verventa(void){
	_SetDesktopColor();
	_CreateWindow();
	_CreateWindowAsChild();
	hWinThermostat = CreateThermostat();
	
	return(SUCCESS);
}