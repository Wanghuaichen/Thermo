#include "adaptive_start.h"
#include <stdio.h>
#include "schedules.h"
#include "program_menu.h"
#include "current_state.h"
#include "i2c.h"
#include "main.h"
#include "led.h"



void schedulesTempChangeSave()
{
	rtc_time_active_t time;
	
	passRTCValues(&time);
	
	if((time.Week_Day == SATURDAY) || (time.Week_Day == SUNDAY))
	{
		if(current_status.isSchedulePerformingWeekEnd) //weekend
		{
			current_status.currentSchedule.week_end.target_celsius = display_status.currentTarget;
		}
		else
		{
			current_status.Temperature.comfort = display_status.currentTarget;
		}
	}
	else
	{
		if(current_status.isSchedulePerformingWeekDay) //weekday
		{
			current_status.currentSchedule.week_day.target_celsius = display_status.currentTarget;
		}
		else
		{
			current_status.Temperature.comfort = display_status.currentTarget;
		}
	}
}


void timeScheduleCheck(program_variables_t week_type, rtc_time_active_t time, bool* isSchedulePerforming)
{
	if(!(*isSchedulePerforming))
	{
		//display_status.currentTarget = current_status.Temperature.comfort;
		if(time.Hours == week_type.start_hour)
		{
			if(time.Minutes == week_type.start_minute)
			{
				*isSchedulePerforming = true;
				display_status.currentTarget = week_type.target_celsius;
				startAdaptiveStart = false;
			}
		}
	}
	else
	{
		if(time.Hours == week_type.end_hour)
		{
			if(time.Minutes == week_type.end_minute)
			{
				*isSchedulePerforming = false;
				display_status.currentTarget = current_status.Temperature.comfort;
				startAdaptiveStart = false;
			}
		}
	}
}

void checkForScheduleStart(program_variables_t week_type, rtc_time_active_t time, bool* isSchedulePerforming)
{
	if(!week_type.isProgramOff) //program is on
	{
		timeScheduleCheck(week_type, time, isSchedulePerforming);//
	}
	else //program is off
	{
		display_status.currentTarget = current_status.Temperature.comfort;
	}
}


void checkWeekDayType(rtc_time_active_t time)
{
	
	if((time.Week_Day == SATURDAY) || (time.Week_Day == SUNDAY))
	{
		checkForScheduleStart(current_status.currentSchedule.week_end, time, &current_status.isSchedulePerformingWeekEnd);
	}
	else
	{
		checkForScheduleStart(current_status.currentSchedule.week_day, time, &current_status.isSchedulePerformingWeekDay);
	}
}

void schedulesCheck()
{
	rtc_time_active_t time;
	
	passRTCValues(&time);
	if((current_status.currentProgram == PROGRAM_1_SELECT) || (current_status.currentProgram == PROGRAM_2_SELECT) || (current_status.currentProgram == PROGRAM_3_SELECT))
		checkWeekDayType(time);
}



void fastCheckIfStartNow(program_day_type_t program)
{
	rtc_time_active_t time;
	
	passRTCValues(&time);

	if((time.Week_Day == SATURDAY) || (time.Week_Day == SUNDAY))
	{
		if(!program.week_end.isProgramOff)
		{
			for(uint8_t i = 0; i <= time.Hours; i++)
			{
				if(i == program.week_end.start_hour)
				{
					for(uint8_t j = 0; j <= time.Minutes; j++)
					{
						if(j == program.week_end.start_minute)
						{
							current_status.isSchedulePerformingWeekEnd = true;
							display_status.currentTarget = program.week_end.target_celsius;
						}
					}
				}
				if(i == program.week_end.end_hour)
				{
					for(uint8_t j = 0; j <= time.Minutes; j++)
					{
						if(j == program.week_end.end_minute)
						{
							current_status.isSchedulePerformingWeekEnd = false;
							display_status.currentTarget = current_status.Temperature.comfort;
						}
					}
				}
			}
		}
	}
	else
	{
		if(!program.week_day.isProgramOff)
		{
			for(uint8_t i = 0; i <= time.Hours; i++)
			{
				if(i == program.week_day.start_hour)
				{
					if(program.week_day.start_hour < time.Hours)
						time.Minutes = 59;
					
					for(uint8_t j = 0; j <= time.Minutes; j++)
					{
						if(j == program.week_day.start_minute)
						{
							current_status.isSchedulePerformingWeekDay = true;
							display_status.currentTarget = program.week_day.target_celsius;
						}
					}
				}
				if(i == program.week_day.end_hour)
				{
					if(program.week_day.end_hour < time.Hours)
						time.Minutes = 59;
					
					for(uint8_t j = 0; j <= time.Minutes; j++)
					{
						if(j == program.week_day.end_minute)
						{
							current_status.isSchedulePerformingWeekDay = false;
							display_status.currentTarget = current_status.Temperature.comfort;
						}
					}
				}
			}
		}
	}
}

/* copy schedule times*/
void passCurrentProgramCallback()
{
	switch(current_status.currentProgram)
	{
		case PROGRAM_1_SELECT:
			current_status.currentSchedule = programs.program_1;
			fastCheckIfStartNow(current_status.currentSchedule);
			break;

		case PROGRAM_2_SELECT:
			current_status.currentSchedule = programs.program_2;
			fastCheckIfStartNow(current_status.currentSchedule);
			break;

		case PROGRAM_3_SELECT:
			current_status.currentSchedule = programs.program_3;
			fastCheckIfStartNow(current_status.currentSchedule);
			break;
		
		case OFF_SELECT:
			memset(&current_status.currentSchedule, 1, sizeof(program_day_type_t));
			display_status.currentState = OFF;
			display_status.currentTarget = 0;
			break;

		case CANCEL_PROGRAM_SELECT:
			memset(&current_status.currentSchedule, 1, sizeof(program_day_type_t));
			display_status.currentTarget = current_status.Temperature.comfort;
			break;
		
		default:
			break;
	}
}


