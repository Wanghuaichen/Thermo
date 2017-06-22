#include "flash.h"
#include "led.h"
#include "factory_settings.h"
#include "schedules.h"
#include "button_drv.h"
#include "button_handler.h"
#include "stm32f0xx_hal.h"
#include "display.h"
#include "main.h"
#include "open_window.h"
#include "time_date.h"
#include "adaptive_start.h"
#include "temperature.h"
#include "current_state.h"
#include "programs.h"

#define BUTTON1_MASK (BIT0 << BUTTON1_GPIO)
#define BUTTON2_MASK (BIT0 << BUTTON2_GPIO)
#define BUTTON3_MASK (BIT0 << BUTTON3_GPIO)

#define MAX_TEMPERATURE_VALUE	35
#define MIN_TEMPERATURE_VALUE 5


void comfortTempSave()
{
	if((current_status.currentProgram == 0) || (current_status.currentProgram == CANCEL_PROGRAM_SELECT))
	{
		current_status.Temperature.comfort = display_status.currentTarget;
	}
}

void IncrementValue()
{
	if(display_status.currentTarget < MAX_TEMPERATURE_VALUE)
		display_status.currentTarget++;
}

void DecrementValue()
{
	if(display_status.currentTarget > MIN_TEMPERATURE_VALUE)
		display_status.currentTarget--;
}

void stateForward()
{
	if(current_status.timeDateWasSet)
	{
	if(display_status.currentState == FACTORY_SETTINGS)
		display_status.currentState = TIME_DATE;
	else 
		display_status.currentState++;
	}
}

void stateBack()
{
	if(current_status.timeDateWasSet)
	{
		if(display_status.currentState == TIME_DATE)
			display_status.currentState = FACTORY_SETTINGS;
		else 
			display_status.currentState--;
	}
}

void powerLossSettings()
{
	display_status.currentState = COMFORT;
	display_status.currentTarget = current_status.Temperature.comfort;
	current_status.timeDateWasSet = false;
	current_status.currentProgram = CANCEL_PROGRAM_SELECT;
	flashWriteHalfword();
}

void  pushButtonUpCallback(push_event_t pushEvent)
{
	
	switch(display_status.currentState)
	{
		case COMFORT:
			if (pushEvent == PUSH_SHORT)
			{
				IncrementValue();
				comfortTempSave();
				LEDSetBlinking();
			}
		break;


		case TIME_DATE:
			if (pushEvent == PUSH_SHORT)
				timeDateButtonHandlerIncrement();
		break;


		case PROGRAM_SELECT:
			if (pushEvent == PUSH_SHORT)
			{
				programPlus();
				settingProgramsButtonIncrease();
			}
		break;


		case OPEN_WINDOW:
			openWindowStateSelect();
		break;


		case LOCK:
			if (pushEvent == PUSH_SHORT)
				displayLockedScreen();
		break;


		case CALIBRATION:
			IncrementCalibrationValue();
		break;


		case ADAPTIVE_START:
			adaptiveStartStateSelect();
		break;


		case FACTORY_SETTINGS:
		break;


		case ANTIFREEZE:
		break;


		case OFF:
		break;


		case POWER_LOSS:
			powerLossSettings();
		break;
	}
}

void  pushButtonDownCallback(push_event_t pushEvent)
{
	
	switch(display_status.currentState)
	{
		case COMFORT:
			if (pushEvent == PUSH_SHORT)
			{
				DecrementValue();
				comfortTempSave();
				LEDSetBlinking();
			}
		break;


		case TIME_DATE:
			if (pushEvent == PUSH_SHORT)
				timeDateButtonHandlerDecrement();
		break;


		case PROGRAM_SELECT:
			if (pushEvent == PUSH_SHORT)
			{
				programMinus();
				settingProgramsButtonDecrease();
			}
		break;


		case OPEN_WINDOW:
			openWindowStateSelect();
		break;


		case LOCK:
			if (pushEvent == PUSH_SHORT)
				displayLockedScreen();
		break;


		case CALIBRATION:
			DecrementCalibrationValue();
		break;


		case ADAPTIVE_START:
			adaptiveStartStateSelect();
		break;


		case FACTORY_SETTINGS:
		break;


		case ANTIFREEZE:
			// Brute Force?
		break;


		case OFF:
			// is this needed?
		break;


		case POWER_LOSS:
			powerLossSettings();
		break;
	}
}


void pushButtonForwardCallback(push_event_t pushEvent)
{
	switch(display_status.currentState)
	{
		case COMFORT:
			display_status.currentState = PROGRAM_MENU;
			programMenuIncrease();
		break;


		case TIME_DATE:
			if ((pushEvent == PUSH_SHORT) && (display_status.currentTimeDateState == TIME_DATE_MAIN))
				stateForward();
		break;


		case PROGRAM_SELECT:
			if (pushEvent == PUSH_SHORT)
			{
				if(!programs.programActive)
					stateForward();
				nextProgram();
				programsPassAlongSettings();
			}
		break;


		case OPEN_WINDOW:
			if ((pushEvent == PUSH_SHORT) && (!display_status.openWindowState.selectedOpenWindowDisplay))
				stateForward();
		break;


		case LOCK:
			if (pushEvent == PUSH_SHORT)
				displayLockedScreen();
		break;


		case CALIBRATION:
			if ((pushEvent == PUSH_SHORT) && (!calibration_active))
				stateForward();
		break;


		case ADAPTIVE_START:
			if ((pushEvent == PUSH_SHORT) && (!adaptive_start_state.selectedAdaptiveStartDisplay))
				stateForward();
		break;


		case FACTORY_SETTINGS:
			if ((pushEvent == PUSH_SHORT) && (!factorySettingsActive))
				stateForward();
		break;


		case ANTIFREEZE:
		break;


		case OFF:
			display_status.currentState = PROGRAM_MENU;
			display_status.currentTarget = current_status.Temperature.comfort;
			programMenuIncrease();
		break;


		case POWER_LOSS:
			powerLossSettings();
		break;
		
		case PROGRAM_MENU:
			programMenuIncrease();
		break;
	}
}


void  pushButtonBackCallback(push_event_t pushEvent)
{
	switch(display_status.currentState)
	{
		case COMFORT:
		
		break;


		case TIME_DATE:
			if ((pushEvent == PUSH_SHORT) && (display_status.currentTimeDateState == TIME_DATE_MAIN))
				stateBack();
		break;


		case PROGRAM_SELECT:
			if (pushEvent == PUSH_SHORT)
			{
				if(!programs.programActive)
					stateBack();
				previousProgram();
			}
		break;


		case OPEN_WINDOW:
			if ((pushEvent == PUSH_SHORT)  && (!display_status.openWindowState.selectedOpenWindowDisplay))
				stateBack();
		break;


		case LOCK:
			if (pushEvent == PUSH_SHORT)
				displayLockedScreen();
		break;


		case CALIBRATION:
			if ((pushEvent == PUSH_SHORT) && (!calibration_active))
				stateBack();
		break;


		case ADAPTIVE_START:
			if ((pushEvent == PUSH_SHORT) && (!adaptive_start_state.selectedAdaptiveStartDisplay))
				stateBack();
		break;


		case FACTORY_SETTINGS:
			if ((pushEvent == PUSH_SHORT) && (!factorySettingsActive))
				stateBack();
		break;


		case ANTIFREEZE:
		break;


		case OFF:
		break;


		case POWER_LOSS:
			powerLossSettings();
		break;
		
		case PROGRAM_MENU:
			programMenuDecrease();
		break;
	}
}


void  multiPushButtonHOCallback(void)
{
	
	
}



void pushButtonFunctionCallback(push_event_t pushEvent)
{
	
	switch(display_status.currentState)
	{
		case COMFORT:
			if (pushEvent == PUSH_HOLD_STOP)
			{
				display_status.currentState = TIME_DATE;
			}
			else if (pushEvent == PUSH_SHORT)
			{
				schedulesTempChangeSave();
				doBlinkingLED = false;
			}
		break;


		case TIME_DATE:
			if (pushEvent == PUSH_HOLD_STOP)
			{
				display_status.currentState = COMFORT;
				display_status.currentTimeDateState = TIME_DATE_MAIN;
			}
			else if (pushEvent == PUSH_SHORT)
			{
				timeDateButtonState();
			}
		break;


		case PROGRAM_SELECT:
			if (pushEvent == PUSH_HOLD_STOP)
			{
				display_status.currentState = COMFORT;
				programs.programActive = 0;
				programs.program_settings = START;
			}
			else if (pushEvent == PUSH_SHORT)
			{
				programActiveState();
			}
		break;


		case OPEN_WINDOW:
			if (pushEvent == PUSH_HOLD_STOP)
			{
				display_status.currentState = COMFORT;
			}
			else if (pushEvent == PUSH_SHORT)
			{
				openWindowStateDisplay();
			}
		break;


		case LOCK:
			if (pushEvent == PUSH_SHORT)
				displayLockedScreen();
		break;


		case CALIBRATION:
			if (pushEvent == PUSH_HOLD_STOP)
			{
				display_status.currentState = COMFORT;
			}

			if (pushEvent == PUSH_SHORT)
			{
				SwitchCalibrationState();
			}
		break;


		case ADAPTIVE_START:
			if (pushEvent == PUSH_HOLD_STOP)
			{
				display_status.currentState = COMFORT;
			}
			if (pushEvent == PUSH_SHORT)
			{
				adaptiveStartStateDisplay();
			}
		break;


		case FACTORY_SETTINGS:
			if ((pushEvent == PUSH_HOLD_STOP) && (!factorySettingsActive))
			{
				display_status.currentState = COMFORT;
			}
			if (pushEvent == PUSH_SHORT)
			{
				factorySettingsStateDisplay();
			}
		break;


		case ANTIFREEZE:
			if (pushEvent == PUSH_HOLD_STOP)
			{
				closingTheWindow();
			}
		break;


		case OFF:
			//is this needed?
		break;


		case POWER_LOSS:
			powerLossSettings();
		break;
		
		case PROGRAM_MENU:
			if (pushEvent == PUSH_HOLD_STOP)
			{
				programMenuDecrease();
				programMenuSetComfort();
				//TODO back to faze one to all
			}
			if (pushEvent == PUSH_SHORT)
			{
				current_status.isSchedulePerformingWeekDay = false;
				current_status.isSchedulePerformingWeekEnd = false;
				programMenuSetCurrentProgram();
				programMenuDecrease();
				if(display_status.currentState != OFF)
					display_status.currentState = COMFORT;
				schedulesCheck();
			}
		break;
	}
}

void  buttons_init(void)
{
	
	//button_drv_init(multiPushButtonOFCallback);
	button_drv_init(multiPushButtonHOCallback);
	
	
	register_push_button(SW1_Pin,
		1,
		54, 
		250, 
		pushButtonUpCallback);
	enable_push_button(SW1_Pin, true);
	
	register_push_button(SW2_Pin,
		1,
		54, 
		250,
		pushButtonDownCallback);
	enable_push_button(SW2_Pin, true);
	
	register_push_button(SW3_Pin,
		2,
		250, 
		500,
		pushButtonBackCallback);
	enable_push_button(SW3_Pin, true);
	
	register_push_button(SW4_Pin,
		2,
		250, 
		500,
		pushButtonForwardCallback);
	enable_push_button(SW4_Pin, true);
	
	register_push_button(SW5_Pin,
		2, 
		100,
		250,
		pushButtonFunctionCallback);
	enable_push_button(SW5_Pin, true);
	
/*	register_push_button(SW3_Pin|SW5_Pin,
		10,
		500, 
		1000,
		multiPushButtonOFCallback);
	enable_push_button(SW3_Pin|SW5_Pin, true);
	
	
	register_push_button(SW4_Pin|SW5_Pin,
		10,
		500, 
		1000,
		multiPushButtonHOCallback);
	enable_push_button(SW4_Pin|SW5_Pin, true);  */
}		
