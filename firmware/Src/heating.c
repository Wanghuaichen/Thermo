#include "adaptive_start.h"
#include "heating.h"
#include "macros.h"
#include "temperature.h"
#include "hardware_test.h"
#include "current_state.h"

#define HEATING_TIME 	480


bool triac = false;
bool refreshPWMcount = false;
bool DutyCylePercentage = false;


void triacControl(int16_t target_temperature)
{
	static uint16_t temp_dif = 0;
	static uint16_t temp_pwm = 0;
	
	if ((Calculated_temperature < target_temperature+5) && (!isTestMode))
	{
		if (temp_pwm == 1)
		{
			temp_dif = (target_temperature+5) - Calculated_temperature;
			if(temp_dif>20) temp_dif=20;
			temp_dif = temp_dif * 96;
			
			if (temp_dif >= 1920) 
				DutyCylePercentage = true;
			else
				DutyCylePercentage = false;
		}
		
		if(startAdaptiveStart)
			temp_dif = 1920;
		
		if (temp_pwm < temp_dif)
		{
			SET_TRIAC;
			current_status.heatingIsON = true;
		}

		else if(temp_pwm < 1920)
		{
			RESET_TRIAC;
			current_status.heatingIsON = false;
		}
		
		if ( temp_pwm >= 1920)
		{
			temp_pwm = 0;
			RESET_TRIAC;
			current_status.heatingIsON = false;
		}
		
		temp_pwm++;
	}
	else
	{
		current_status.heatingIsON = false;
		RESET_TRIAC;
		DutyCylePercentage = false;
	}
}

void heatingSetTargetTemperature(int16_t temperature_to_targeted)
{
	triacControl(temperature_to_targeted);
}

