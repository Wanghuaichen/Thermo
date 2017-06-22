#include "hardware_test.h"
#include "temperature.h"
#include "macros.h"
#include "display.h"
#include "i2c.h"

bool isTestMode = false;
bool counterPassed = false;

void hardwareTest()
{
	uint8_t bit_check = 0;
	LED_1_2_OFF;
	LED_3_OFF;
	uint8_t slave_address = i2cReturnSlaveAddress();
	
	if((slave_address == 0x00) && (Calculated_temperature == -1))
	{
		while(1)
		{
			displayCharSegment(14, 1);
			for(uint8_t i = 1; i != 0; i++)
			{
			}
		}
	}
	
	if((Calculated_temperature == -1) && (getTemperatureVal) && (!counterPassed))
	{
		display_status.currentState = HARDWARE_TEST;
		isTestMode = true;
		CLEAR_DISPLAY;
		while(1)
		{
			
			if (BUTTON_5_IS_PRESSED)
			{
				bit_check |= 1 << 4;
			}
			if (BUTTON_4_IS_PRESSED)
			{
				bit_check |= 1 << 3;
			}
			if (BUTTON_3_IS_PRESSED)
			{
				bit_check |= 1 << 2;
			}
			if (BUTTON_2_IS_PRESSED)
			{
				bit_check |= 1 << 1;
			}
			if (BUTTON_1_IS_PRESSED)
			{
				bit_check |= 1;
			}
			if(bit_check == 0x1F)
			{
				LED_1_2_ON;
				LED_3_ON;
				SET_ANODE1;
				SET_ANODE2;
				while(1)
				{
					SET_DISPLAY(8);
					isTestMode = false;
				}
			}
		}
	}
	counterPassed = true;
}

