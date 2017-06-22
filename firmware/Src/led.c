#include "led.h"
#include "macros.h"
#include "current_state.h"

bool doToggleLED = false;
bool doBlinkingLED = false;
bool blinkLED = false;
//uint16_t LEDblinkingCounter = 0;

void redLed()
{
	if(current_status.heatingIsON)
	{
		RESET_ANODE2;
		LED_1_2_ON;	
	}
	else
		LED_1_2_OFF;
}

void firstLED()
{
	if(blinkLED)
	{
		RESET_ANODE1;
		LED_1_2_ON;	
	}
}

void secondLED()
{
	if(blinkLED)
	{
		LED_1_2_OFF;
		LED_3_ON;
	}
}
// FAKTORIUS
void ledCheckForProgram()
{
	if((current_status.currentProgram == PROGRAM_1_SELECT) || (current_status.currentProgram == PROGRAM_2_SELECT) || (current_status.currentProgram == PROGRAM_3_SELECT))
		doToggleLED = true;
	else
		doToggleLED = false;
}

void LEDSetBlinking()
{
	if((current_status.currentProgram == PROGRAM_1_SELECT) || (current_status.currentProgram == PROGRAM_2_SELECT) || (current_status.currentProgram == PROGRAM_3_SELECT))
		doBlinkingLED = true;
}

void LEDToggle()
{
	ledCheckForProgram();
	LED_1_2_OFF;
	if(doToggleLED)
	{
		if(current_status.isSchedulePerformingWeekDay || current_status.isSchedulePerformingWeekEnd)
		{
			firstLED();
		}
		else
		{
			secondLED();
		}
	}
}

void LEDBlinkingcount()
{
	static uint8_t LEDblinkingCounter = 0;
	
	if(doBlinkingLED)
	{
		LEDblinkingCounter++;
		
		if(LEDblinkingCounter == 16)
		{
			blinkLED = false;
		}
		if(LEDblinkingCounter >= 32)
		{
			LEDblinkingCounter = 0;
			blinkLED = true;
		}
	}
	else
	{
		LEDblinkingCounter = 0;
		blinkLED = true;
	}
}
