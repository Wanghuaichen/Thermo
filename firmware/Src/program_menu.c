#include "schedules.h"
#include "program_menu.h"
#include "display.h"
#include "current_state.h"


program_menu_t programs_menu;



void programMenuSetComfort()
{
	programs_menu = EMPTY_SPACE;
}

void programMenuSetCurrentProgram()
{
	current_status.currentProgram = programs_menu;
	passCurrentProgramCallback();
}


void programMenuDecrease()
{
	if(programs_menu == PROGRAM_1_SELECT) programs_menu = OVER_THE_LINE;

	programs_menu--;
}

void programMenuIncrease()
{
	programs_menu++;

	if(programs_menu > CANCEL_PROGRAM_SELECT) programs_menu = PROGRAM_1_SELECT;
}


void programMenuSelect()
{
	switch(programs_menu)
	{

		case PROGRAM_1_SELECT:
			displayCharSegment(23, 1);
			break;

		case PROGRAM_2_SELECT:
			displayCharSegment(23, 2);
			break;

		case PROGRAM_3_SELECT:
			displayCharSegment(23, 3);
			break;

		case OFF_SELECT:
			displayCharSegment(0, 14);
			break;

		case CANCEL_PROGRAM_SELECT:
			displayCharSegment(25, 23); //nP
			break;
		
		default:
			break;

	}
}


