

#include "open_window.h"
#include "display.h"
#include "current_state.h"
#include "temperature.h"
#include "tim.h"
#include "heating.h"

#define OPEN_WINDOW_CONSTANT 							50 			// 10 - 15 C should be
#define CLOSED_WINDOW_CONSTANT 						50 			// 10 - 15 C should be
#define CLOSED_WINDOW_TEMPERATURE_VALUE		110 			// WT 1100 DT 110
#define TEMPERATURE_THRESHOLD							10
#define ONE_MINUTE 												16 * 60				//one min
#define FIFTEEN_MINUTES										16 * 60 * 15  //15 min

#define ANTIFREEZE_TEMPERATURE						7

bool open_window_start_timer = false;
uint16_t counter_open_window = 880;
int16_t last_checked_temperature = 0;
bool isWindowOpen = false;


/* This is needed to know the value of starting temperature */
void open_window_init()
{
	last_checked_temperature = Calculated_temperature;
}

void openWindowCheckNotSuccess()
{
	last_checked_temperature = Calculated_temperature;
	open_window_start_timer = false;
	counter_open_window = 0;
}

bool heaterNotHeating()
{
	if ((last_checked_temperature - Calculated_temperature) < 20)
	{
		return 1;
	}
	return 0;
}


void notHeatingCount()
{
	
	static uint8_t vain_heating_count = 0;
	if(DutyCylePercentage)
	{
		if(heaterNotHeating())
			vain_heating_count++;
		else
			vain_heating_count = 0;
	}
	else
		vain_heating_count = 0;

	if (vain_heating_count > 5)
	{
		isWindowOpen = true;
		display_status.currentState = ANTIFREEZE;
		display_status.currentTarget = ANTIFREEZE_TEMPERATURE;
		openWindowCheckNotSuccess();
	}
}


void openWindowSpeculateTemperature()
{
	if(((last_checked_temperature - Calculated_temperature) > TEMPERATURE_THRESHOLD) && (!open_window_start_timer))
	{
		open_window_start_timer = true;
	}
	else 
		last_checked_temperature = Calculated_temperature;
	
	
	notHeatingCount();
	counter_open_window = 0;
}

void openWindowCheckTemperatureLoss()
{
	if ((last_checked_temperature - Calculated_temperature) > OPEN_WINDOW_CONSTANT)
	{
		isWindowOpen = true;
		display_status.currentState = ANTIFREEZE;
		display_status.currentTarget = ANTIFREEZE_TEMPERATURE;
		openWindowCheckNotSuccess();
	}
	
	increment_counter(&counter_open_window);
}



void closedWindowSpeculateTemperature()
{
	if(((Calculated_temperature - last_checked_temperature) > TEMPERATURE_THRESHOLD) && (!open_window_start_timer))
	{
		open_window_start_timer = true;
	}
	else 
		last_checked_temperature = Calculated_temperature;
	counter_open_window = 0;
}

void closedWindowCheckNotSuccess()
{
	last_checked_temperature = Calculated_temperature;
	open_window_start_timer = false;
	counter_open_window = 0;
}

void closingTheWindow()
{
	isWindowOpen = false;
	display_status.currentState = COMFORT;
	display_status.currentTarget = current_status.Temperature.comfort;
	closedWindowCheckNotSuccess();
}

void closedWindowCheckTemperatureLoss()
{
	if ((Calculated_temperature - last_checked_temperature) > CLOSED_WINDOW_CONSTANT)
	{
		closingTheWindow();
	}
	increment_counter(&counter_open_window);
}



void openWindowTimer()
{
	if(current_status.isOpenWindow)
	{
		if (!isWindowOpen)
		{
			if(open_window_start_timer)
			{
				counter_open_window < FIFTEEN_MINUTES ? openWindowCheckTemperatureLoss() : openWindowCheckNotSuccess();
			}
			else
			{
				counter_open_window < ONE_MINUTE ? increment_counter(&counter_open_window) : openWindowSpeculateTemperature();
			}
		}
		else
		{
			if(open_window_start_timer)
			{
				counter_open_window < FIFTEEN_MINUTES ? closedWindowCheckTemperatureLoss() : closedWindowCheckNotSuccess();
			}
			else
			{
				counter_open_window < ONE_MINUTE ? increment_counter(&counter_open_window) : closedWindowSpeculateTemperature();
			}
		}
	}
}

/*******************************************************/

void openWindowStateSelect()
{
	if(display_status.openWindowState.selectedOpenWindowDisplay)
	{
		if(display_status.openWindowState.openWindowState)
			display_status.openWindowState.openWindowState = false;
		else
			display_status.openWindowState.openWindowState = true;
	}
}

void openWindowStateDisplay()
{
	
	if (display_status.openWindowState.selectedOpenWindowDisplay)
	{
		display_status.openWindowState.selectedOpenWindowDisplay = false;
		current_status.isOpenWindow = display_status.openWindowState.openWindowState;
	}
	else
		display_status.openWindowState.selectedOpenWindowDisplay = true;
	
}


