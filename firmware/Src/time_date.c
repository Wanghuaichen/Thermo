
#include "i2c.h"
#include "time_date.h"
#include "main.h"
#include "display.h"
#include "current_state.h"


#define MIN_DATE_DAY 	1
#define MAX_DATE_DAY 	7
#define MIN_TIME_HOUR	0
#define MAX_TIME_HOUR	23
#define MIN_TIME_MIN	0
#define MAX_TIME_MIN	59 // based on the array below

time_date_info_t time_date_info;
//current_status_t current_status;


void timeDateIncrement(uint16_t low, uint16_t high, uint16_t *variable)
{
	if(*variable == high)
		*variable = low;
	else
		*variable += 1;
}

void timeDateDecrement(uint16_t low, uint16_t high, uint16_t *variable)
{
	if(*variable == low)
		*variable = high;
	else
		*variable -= 1;
}

void timeDateButtonHandlerIncrement()
{
	switch(display_status.currentTimeDateState)
	{
		case TIME_DATE_MAIN:
			break;

		case TIME_DATE_WEEK_DAY:
			timeDateIncrement(MIN_DATE_DAY, MAX_DATE_DAY, (uint16_t*)&time_date_info.day);
			break;

		case TIME_DATE_HOUR:
			timeDateIncrement(MIN_TIME_HOUR, MAX_TIME_HOUR, &time_date_info.hour);
			break;

		case TIME_DATE_MINUTE:
			timeDateIncrement(MIN_TIME_MIN, MAX_TIME_MIN, &time_date_info.minute);
			break;

		default:	
			break;
	}
}


void timeDateButtonHandlerDecrement()
{
	switch(display_status.currentTimeDateState)
	{
		case TIME_DATE_MAIN:
			break;

		case TIME_DATE_WEEK_DAY:
			timeDateDecrement(MIN_DATE_DAY, MAX_DATE_DAY, (uint16_t*)&time_date_info.day);
			break;

		case TIME_DATE_HOUR:
			timeDateDecrement(MIN_TIME_HOUR, MAX_TIME_HOUR, &time_date_info.hour);
			break;

		case TIME_DATE_MINUTE:
			timeDateDecrement(MIN_TIME_MIN, MAX_TIME_MIN, &time_date_info.minute);
			break;

		default:	
			break;
	}
}



void timeDateButtonState()
{
	display_status.currentTimeDateState++;
}

bool changedSettingsCheck()
{
	bool isChanged = false;
	if (!(time_date_info.day == current_status.CurrentTimeDate.day))
	{
		current_status.CurrentTimeDate.day = time_date_info.day;
		isChanged = true;
	}
	if (!(time_date_info.hour == current_status.CurrentTimeDate.hour))
	{
		current_status.CurrentTimeDate.hour = time_date_info.hour;
		isChanged = true;
	}
	if (!(time_date_info.minute == current_status.CurrentTimeDate.minute))
	{
		current_status.CurrentTimeDate.minute = time_date_info.minute;
		isChanged = true;
	}
	return isChanged;
}


void timeDateDisplay(display_status_t *display_status)
{
	switch(display_status->currentTimeDateState)
	{
		case TIME_DATE_MAIN:
			displayCharSegment(22, 13);
			break;

		case TIME_DATE_WEEK_DAY:
			displayNumberSegment(time_date_info.day);
			break;

		case TIME_DATE_HOUR:
			displayNumberSegment(time_date_info.hour);
			break;

		case TIME_DATE_MINUTE:
			displayNumberSegment(time_date_info.minute);
			break;

		default:	
			if (changedSettingsCheck())
			{
				i2cWriteTimeToRTC(time_date_info.minute, time_date_info.hour, time_date_info.day);
				current_status.timeDateWasSet = true;
				//send to RTC
			}
			display_status->currentTimeDateState = TIME_DATE_MAIN;
			break;
	}
}

void timeDateInit()
{
	time_date_info.day = TUESDAY;
	time_date_info.hour = 16;
	time_date_info.minute = 30;
}
