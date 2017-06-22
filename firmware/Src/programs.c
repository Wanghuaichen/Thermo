#include "programs.h"
#include "display.h"
#include <stdio.h>
#include "current_state.h"


#define MAX_PROGRAM_ACTIVES				5

uint8_t day_type[] = { 5, 2 };

uint16_t minute_number[] =
{
	0, 15, 30, 45
};

programs_t programs;
program_variables_t setting_var;

void programsInit(bool doSetDefault)
{
	programs.programActive = false;
	programs.program_select = PROGRAM_1;
	programs.program_settings = START;
	
	setting_var.start_hour = 12;
	setting_var.start_minute = 2;
	setting_var.end_hour = 20;
	setting_var.end_minute = 2;
	setting_var.target_celsius = 16;
	setting_var.isProgramOff = true;
	
	if(doSetDefault)
	{
		programs.program_1.week_day = setting_var;
		programs.program_1.week_end = setting_var;
		programs.program_2.week_day = setting_var;
		programs.program_2.week_end = setting_var;
		programs.program_3.week_day = setting_var;
		programs.program_3.week_end = setting_var;
	}
}

void programTypeSwitch()
{
	if(programs.programActive == 2)
	{
		if (programs.program_type)
			programs.program_type = WEEKDAY;
		else
			programs.program_type = WEEKEND;
	}
}

void programsCheckForLowestTarget()
{
	uint8_t lowestTarget = 0xFF;
	
	if (current_status.Temperature.comfort < lowestTarget)
		lowestTarget = current_status.Temperature.comfort;
	
	if (programs.program_1.week_day.target_celsius < lowestTarget)
		lowestTarget = programs.program_1.week_day.target_celsius;
	
	if (programs.program_1.week_end.target_celsius < lowestTarget)
		lowestTarget = programs.program_1.week_end.target_celsius;
	
	if (programs.program_2.week_day.target_celsius < lowestTarget)
		lowestTarget = programs.program_2.week_day.target_celsius;
	
	if (programs.program_2.week_end.target_celsius < lowestTarget)
		lowestTarget = programs.program_2.week_end.target_celsius;
	
	if (programs.program_3.week_day.target_celsius < lowestTarget)
		lowestTarget = programs.program_3.week_day.target_celsius;
	
	if (programs.program_3.week_end.target_celsius < lowestTarget)
		lowestTarget = programs.program_3.week_end.target_celsius;
	
	display_status.currentTarget = lowestTarget;
}

void startOrOff()
{
	if(programs.programActive == 3)
	{
		if(programs.program_settings)
		{
			programs.program_settings = NO_PROGRAM; //off
		}
		else
		{
			programs.program_settings = START;
		}
	}
}

void programPlus()
{
	 programTypeSwitch();
	 startOrOff();
}

void programMinus()
{
	 programTypeSwitch();
	 startOrOff();
}

void nextProgram()
{
	if(programs.programActive == 1)
	{
		programs.program_select++;
		if(programs.program_select > PROGRAM_3) programs.program_select = PROGRAM_1;
	}
}

void previousProgram()
{
	if(programs.programActive == 1)
	{
		programs.program_select--;
		if(programs.program_select < PROGRAM_1) programs.program_select = PROGRAM_3;
	}
}

void programsPassAlongSettings()
{
	if(programs.programActive > 2)
	{
		programs.program_settings++;
		if(programs.program_settings > TEMPERATURE) programs.program_settings = NO_PROGRAM;
	}
}

void noProgram(bool* doSetOff)
{
	if (programs.programActive == 3)
	{
		displayCharSegment(26, 14);
		*doSetOff = true;
	}
	if (programs.programActive == 4)
	{
		programs.programActive = 0;
		//programActiveState();
		programs.program_settings = START;
	}
}

void startProgram(bool* doSetOn)
{
	if(programs.programActive == 3)
	{
		displayCharSegment(5, 22);
		*doSetOn = false;
	}
	if(programs.programActive == 4)
	{
		programs.program_settings++;
	}
}

void hourSet(uint8_t *hour_set, uint8_t hour_to_set)
{
	if(programs.programActive == 3)
		programs.programActive++;
	
	*hour_set = hour_to_set;
	displayNumberSegment(*hour_set);
}

void minuteTempSet(uint8_t *minute_set, uint8_t minute_to_set)
{
	*minute_set = minute_number[minute_to_set];
	displayNumberSegment(*minute_set);
}

void TempSet(uint8_t *temp_set, uint8_t temp_to_set)
{
	*temp_set = temp_to_set;
	displayNumberSegment(*temp_set);
}

void settingProgramsDisplay(program_variables_t *week_day_settings)
{
	switch(programs.program_settings)
	{
		case NO_PROGRAM:
			noProgram(&week_day_settings->isProgramOff);
			break;

		case START:
			startProgram(&week_day_settings->isProgramOff);
			break;

		case START_HOUR:
			hourSet(&week_day_settings->start_hour, setting_var.start_hour);
			break;

		case START_MINUTE:
			minuteTempSet(&week_day_settings->start_minute, setting_var.start_minute);
			break;

		case END:
			displayCharSegment(21, 25);
			break;

		case END_HOUR:
			hourSet(&week_day_settings->end_hour, setting_var.end_hour);
			break;

		case END_MINUTE:
 			minuteTempSet(&week_day_settings->end_minute, setting_var.end_minute);
			break;

		case CELSIUS:
			displayCharSegment(18, 12);
			break;

		case TEMPERATURE:
			TempSet(&week_day_settings->target_celsius, setting_var.target_celsius);
			break;
		
		default:
			break;
	}
}

void programsIncrement(uint8_t* incrementing_variable, uint8_t max_value, uint8_t min_value)
{
	*incrementing_variable += 1;
	
	if (*incrementing_variable > max_value)
		*incrementing_variable = min_value;
}

void programsDecrement(uint8_t* decrementing_variable, uint8_t max_value, uint8_t min_value)
{
	if (*decrementing_variable == min_value)
		*decrementing_variable = max_value + 1;

		*decrementing_variable -= 1;
}


void settingProgramsButtonIncrease()
{
	switch(programs.program_settings)
	{
		case START_HOUR:
			programsIncrement(&setting_var.start_hour, 23, 0);
			break;

		case START_MINUTE:
			programsIncrement(&setting_var.start_minute, 3, 0);
			break;

		case END_HOUR:
			programsIncrement(&setting_var.end_hour, 23, 0);
			break;

		case END_MINUTE:
 			programsIncrement(&setting_var.end_minute, 3, 0);
			break;

		case TEMPERATURE:
			programsIncrement(&setting_var.target_celsius, 35, 5);
			break;
		
		default:
			break;
	}
}

void settingProgramsButtonDecrease()
{
	switch(programs.program_settings)
	{
		case START_HOUR:
			programsDecrement(&setting_var.start_hour, 23, 0);
			break;

		case START_MINUTE:
			programsDecrement(&setting_var.start_minute, 3, 0);
			break;

		case END_HOUR:
			programsDecrement(&setting_var.end_hour, 23, 0);
			break;

		case END_MINUTE:
 			programsDecrement(&setting_var.end_minute, 3, 0);
			break;

		case TEMPERATURE:
			programsDecrement(&setting_var.target_celsius, 35, 5);
			break;
		
		default:
			break;
	}
}

void settingWeekDay(program_day_type_t *program_type_weekday)
{
	if(programs.program_type == WEEKDAY)
		settingProgramsDisplay(&program_type_weekday->week_day);
	else if(programs.program_type == WEEKEND)
		settingProgramsDisplay(&program_type_weekday->week_end);
}

void programSelect(program_day_type_t *program_selecting)
{
	if(programs.programActive == 1)
		displayCharSegment(23, (uint8_t)programs.program_select);
	else if(programs.programActive == 2)
		displayCharSegment(18,  day_type[programs.program_type]);
	else if(programs.programActive == 3)
		settingWeekDay(program_selecting);
	else if(programs.programActive == 4)
		settingWeekDay(program_selecting);

}

void programStateDisplay(programs_t *programs)
{
	if (programs->program_select == 0)
		programs->program_select++;
	switch(programs->program_select)
	{
		case PROGRAM_1:
			programSelect(&programs->program_1);
			break;

		case PROGRAM_2:
			programSelect(&programs->program_2);
			break;

		case PROGRAM_3:
			programSelect(&programs->program_3);
			break;
	}
}
/*
void setActiveProgramOnStartup()
{
	
}
*/

void programActiveState()
{
	if(programs.programActive != 4)
		programs.programActive++;

	if(programs.programActive >= MAX_PROGRAM_ACTIVES)
		programs.programActive = 0;
}


void programDisplay()
{

	if(programs.programActive != 0)
	{
		programStateDisplay(&programs);
	}
	else
	{
		displayCharSegment(23, 20);
	}
}
