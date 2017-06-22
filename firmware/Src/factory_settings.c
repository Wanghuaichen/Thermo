#include "flash.h"
#include "factory_settings.h"
#include "macros.h"

#define FACTORY_RESET_COUNTER 75000 //20000 - cute, 10000 - loading, 75000 - best?

bool factorySettingsActive = false;
bool factorySettingsDownTheDrainHole = false;
bool wasPressed = false;

uint32_t count = 0;

uint16_t factorySettingsSetRound = 0;
uint16_t factorySettingsRound[] = 
{
	0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFA, 0xFB
};

void factorySettingsDisplayTimer()
{
	
	if (BUTTON_5_IS_PRESSED)
	{
		wasPressed = true;
		count++;
		if(count == FACTORY_RESET_COUNTER)
		{
			count = 0;
			factorySettingsSetRound++;
			if (factorySettingsSetRound >= 7)
			{
				flashFullErase();
				HAL_NVIC_SystemReset();
			}
		}
	}
	else
	{
		count = 0;
		factorySettingsSetRound = 0;
		
		if(wasPressed)
		{
			if(!BUTTON_5_IS_PRESSED)
			{
				factorySettingsActive = false;
				wasPressed = false;
			}
		}
		
		factorySettingsSetRound = 0;
	}
}

void factorySettingsStateDisplay()
{

	if(!factorySettingsActive)
	{
		factorySettingsActive = true;
		CLEAR_DISPLAY;
		SET_ANODE1;
		SET_ANODE2;
	}
	else
	{
		factorySettingsActive = false;
		wasPressed = false;
	}
}

