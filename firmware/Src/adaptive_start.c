#include "i2c.h"
#include "temperature.h"
#include "heating.h"
#include "adaptive_start.h"
#include "display.h"
#include "current_state.h"

#define ALFA_CONSTANT 0.8

adaptive_start_state_t adaptive_start_state;
uint32_t timeStampStart = 0;
uint16_t timeStamp = 0;
bool startAdaptiveStart = false;


void adaptiveStartStateSelect()
{
	if(adaptive_start_state.selectedAdaptiveStartDisplay)
	{
		if(adaptive_start_state.adaptiveStartStatus)
			adaptive_start_state.adaptiveStartStatus = false;
		else
			adaptive_start_state.adaptiveStartStatus = true;
	}
}


void adaptiveStartStateDisplay()
{
	if (adaptive_start_state.selectedAdaptiveStartDisplay)
	{
		adaptive_start_state.selectedAdaptiveStartDisplay = false;
		current_status.isAdaptiveStart = adaptive_start_state.adaptiveStartStatus;
	}
	else
		adaptive_start_state.selectedAdaptiveStartDisplay = true;
}

/***************************************************/


bool checkForTimestamp(bool isStarted)
{
	static uint16_t started_temperature = 0;
	
	if(isStarted)
		started_temperature = Calculated_temperature;
	
	else
		if(Calculated_temperature >= started_temperature + 10)
			return true;
		
	return false;
}

void timeStampGet()
{
	static bool isStarted = false;
	static bool isFirst = true;
	static bool newStart = true;
	static uint32_t time_stamp = 0;
	
	if(DutyCylePercentage)
	{
		if(!isStarted)
		{
			time_stamp = 0;
			isStarted = true;
		}
		else
		{
			time_stamp++;
			if(checkForTimestamp(newStart))
			{
				if(isFirst)
				{
					timeStampStart = time_stamp;
					isFirst = false;
				}
				else
					timeStampStart = (ALFA_CONSTANT * timeStampStart) + ((1-ALFA_CONSTANT) * time_stamp); 

				isStarted = false;
				newStart = true;
			}
			if(time_stamp >= 9600)
			{
				isStarted = false;
				newStart = true;
			}
			newStart = false;
		}
	}
	else
	{
		time_stamp = 0;
	}
}

void timeStampToTime()
{
	uint8_t seconds = 0;
	timeStamp = timeStampStart / 16;
	seconds = timeStamp % 60;
	timeStamp = timeStamp / 60;
	if(seconds > 30)
		timeStamp++;
}

uint16_t differenceTargetTemp(uint16_t targert_temp)
{
	uint16_t minuteBefore = 0;
	uint16_t difference = 0;
	
	timeStampToTime();
	
	if(targert_temp > Calculated_temperature)
		difference = targert_temp - Calculated_temperature; 
	if(difference > 20)
		minuteBefore = difference * timeStamp;
	else
		minuteBefore = timeStamp;
	
	if(minuteBefore >= 120)
		minuteBefore = 119;
	
	return minuteBefore;
}

void timeChecking(program_variables_t week_type_select, rtc_time_active_t time, bool schedulePerforme)
{

	uint16_t manyMinutesBefore = timeStamp;
	/** this is if schedule is off **/
	if(schedulePerforme)
	{
		manyMinutesBefore = differenceTargetTemp(week_type_select.target_celsius);
		if(manyMinutesBefore > 59)
		{
			manyMinutesBefore = manyMinutesBefore % 60; 
			if(week_type_select.start_hour != 0)
				week_type_select.start_hour--;
			else week_type_select.start_hour = 23;
		}
		/******************************************************/

		if(week_type_select.start_minute >= manyMinutesBefore)
		{
		// if time is good for subtraction
			if(time.Hours == week_type_select.start_hour)
			{
				if(time.Minutes == (week_type_select.start_minute - manyMinutesBefore))
					startAdaptiveStart = true;
			}
		}
		else
		{
			if(week_type_select.start_hour != 0)
				week_type_select.start_hour--;
			else week_type_select.start_hour = 23;
			
			if(time.Hours == week_type_select.start_hour)
			{
				if(time.Minutes == (60 - manyMinutesBefore + week_type_select.start_minute))
					startAdaptiveStart = true;
			}
		}
	}
	
	/** this is if schedule is on **/
	
	else if(schedulePerforme)
	{
		manyMinutesBefore = differenceTargetTemp(current_status.Temperature.comfort);
		if(manyMinutesBefore > 59)
		{
			manyMinutesBefore = manyMinutesBefore % 60; 
			if(week_type_select.end_hour != 0)
				week_type_select.end_hour--;
			else week_type_select.end_hour = 23;
		}

		if(week_type_select.end_minute >= manyMinutesBefore)
		{
		// if time is good for subtraction
			if(time.Hours == week_type_select.end_hour)
			{
				if(time.Minutes == (week_type_select.end_minute - manyMinutesBefore))
					startAdaptiveStart = true;
			}
		}
		else
		{
			if(week_type_select.end_hour != 0)
				week_type_select.end_hour--;
			else week_type_select.end_hour = 23;
			
			if(time.Hours == week_type_select.end_hour)
			{
				if(time.Minutes == (60 - manyMinutesBefore + week_type_select.end_minute))
					startAdaptiveStart = true;
			}
		}
	}
}

void adaptiveStartTimeCheck()
{
	rtc_time_active_t RTC_Time;
	passRTCValues(&RTC_Time);
	if(current_status.isAdaptiveStart)
	{
		if(startAdaptiveStart == false)
		{
			if((RTC_Time.Week_Day == SATURDAY) || (RTC_Time.Week_Day == SUNDAY))
			{
				timeChecking(current_status.currentSchedule.week_end, RTC_Time, current_status.isSchedulePerformingWeekEnd);
			}
			else
			{
				timeChecking(current_status.currentSchedule.week_day, RTC_Time, current_status.isSchedulePerformingWeekDay);
			}
		}
	}
}
