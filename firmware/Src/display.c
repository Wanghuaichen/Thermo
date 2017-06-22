/*******


*******/
#include "program_menu.h"
#include "tim.h"
#include "time_date.h"
#include "adaptive_start.h"
#include "factory_settings.h"
#include "macros.h"
#include "temperature.h"
#include "display.h"
#include "programs.h"
#include "led.h"

#define MAX_NUMBER_DISPLAY_BLANKS 	32
#define BLINKING_INTERVAL						1700
#define LOCKED_SCREEN_UPON_BUTTON		300000
#define TRIPLE_BLINKING_SCREEN			300000

temperature_state_t tempProgramState;


display_status_t	display_status;

uint16_t screen_toggle_rate = 4;
uint16_t screen_toggle_var[5] =
{
	1, 2, 4, 8, 16
};

uint16_t segments[] = 
{
				  (SEGM_A_Pin | SEGM_B_Pin | SEGM_C_Pin | SEGM_D_Pin | SEGM_E_Pin | SEGM_F_Pin), // 0
											  	 					  (SEGM_B_Pin | SEGM_C_Pin), // 1
				 			   (SEGM_A_Pin | SEGM_B_Pin | SEGM_D_Pin | SEGM_E_Pin | SEGM_G_Pin), // 2
							   (SEGM_A_Pin | SEGM_B_Pin | SEGM_C_Pin | SEGM_D_Pin | SEGM_G_Pin), // 3
							  				(SEGM_B_Pin | SEGM_C_Pin | SEGM_F_Pin | SEGM_G_Pin), // 4
					 		   (SEGM_A_Pin | SEGM_C_Pin | SEGM_D_Pin | SEGM_F_Pin | SEGM_G_Pin), // 5
				  (SEGM_A_Pin | SEGM_C_Pin | SEGM_D_Pin | SEGM_E_Pin | SEGM_F_Pin | SEGM_G_Pin), // 6
													   	 (SEGM_A_Pin | SEGM_B_Pin | SEGM_C_Pin), // 7
	 (SEGM_A_Pin | SEGM_B_Pin | SEGM_C_Pin | SEGM_D_Pin | SEGM_E_Pin | SEGM_F_Pin | SEGM_G_Pin), // 8
				  (SEGM_A_Pin | SEGM_B_Pin | SEGM_C_Pin | SEGM_D_Pin | SEGM_F_Pin | SEGM_G_Pin), // 9
											(SEGM_C_Pin | SEGM_E_Pin | SEGM_F_Pin | SEGM_G_Pin), // h - 10
											(SEGM_A_Pin | SEGM_B_Pin | SEGM_F_Pin | SEGM_G_Pin), // o (laipsniu) - 11
											(SEGM_A_Pin | SEGM_D_Pin | SEGM_E_Pin | SEGM_F_Pin), // C - 12
							   (SEGM_B_Pin | SEGM_C_Pin | SEGM_D_Pin | SEGM_E_Pin | SEGM_G_Pin), // d - 13
											(SEGM_A_Pin | SEGM_E_Pin | SEGM_F_Pin | SEGM_G_Pin), // F - 14
							   (SEGM_B_Pin | SEGM_C_Pin | SEGM_E_Pin | SEGM_F_Pin | SEGM_G_Pin), // H - 15
														 (SEGM_D_Pin | SEGM_E_Pin | SEGM_G_Pin), // c - 16
					  	 	 	 	 	 	  			 (SEGM_D_Pin | SEGM_E_Pin | SEGM_F_Pin), // L - 17
	~(SEGM_A_Pin | SEGM_B_Pin | SEGM_C_Pin | SEGM_D_Pin | SEGM_E_Pin | SEGM_F_Pin | SEGM_G_Pin), // NUL - 18
																				   (SEGM_G_Pin), // - - 19
																	  (SEGM_E_Pin | SEGM_G_Pin), // r - 20
							   (SEGM_A_Pin | SEGM_D_Pin | SEGM_E_Pin | SEGM_F_Pin | SEGM_G_Pin), // E - 21
							   				(SEGM_D_Pin | SEGM_E_Pin | SEGM_F_Pin | SEGM_G_Pin), // t - 22
							   (SEGM_A_Pin | SEGM_B_Pin | SEGM_E_Pin | SEGM_F_Pin | SEGM_G_Pin), // P - 23
				  (SEGM_A_Pin | SEGM_B_Pin | SEGM_C_Pin | SEGM_E_Pin | SEGM_F_Pin | SEGM_G_Pin), // A - 24
				  										 (SEGM_C_Pin | SEGM_E_Pin | SEGM_G_Pin), // n - 25
				  							(SEGM_C_Pin | SEGM_D_Pin | SEGM_E_Pin | SEGM_G_Pin), // o - 26
				  										 (SEGM_C_Pin | SEGM_D_Pin | SEGM_E_Pin), // u - 27

	
};

uint16_t display_segment_switch[] =
{
	LEFT,	BLANK,	
	BLANK,  BLANK,  
	BLANK,  BLANK,  
	BLANK,  BLANK,  
	
	BLANK,	BLANK,	
	BLANK,  BLANK,  
	BLANK,  BLANK,  
	BLANK,  BLANK,  
	
	RIGHT,	BLANK,	
	BLANK,  BLANK,  
	BLANK,  BLANK,  
	BLANK,  BLANK,  
	
	BLANK,	BLANK,	
	BLANK,  BLANK,  
	BLANK,  BLANK,  
	BLANK,  BLANK,  
};


/* if needed */
void display_init(void)
{
	display_status.currentTarget = 20;
	display_status.segmentState = 0;
	display_status.openWindowState.openWindowState = 0;
	display_status.openWindowState.selectedOpenWindowDisplay = 0;

}

bool displayDecrementScreenRate()
{
	screen_toggle_rate--;
	
	if (screen_toggle_rate == 0)
		return false;
	else
		return true;
}

bool displayActivateScreen()
{
	bool wasAsleep = false;
	if(screen_toggle_rate == 0)
		wasAsleep = true;
	screen_toggle_rate = 4;
	tim_set_restart();
	
	return wasAsleep;
}

uint16_t SegmentA(uint16_t number_to_display)
{
	uint16_t Aseg = (number_to_display % 100) /10;
	return Aseg;
}

uint16_t SegmentB(uint16_t number_to_display)
{
	uint16_t Bseg = number_to_display % 10;
	return Bseg;
}

void displayNumberSegment(uint16_t number_to_display)
{
	
	if (display_status.segmentState >= MAX_NUMBER_DISPLAY_BLANKS)
				display_status.segmentState = 0;
	
	switch (display_segment_switch[display_status.segmentState])
	{
		case LEFT: //Segment A

			CLEAR_DISPLAY;
			RESET_ANODE2;
			redLed();
			SET_ANODE1;
			
			if (SegmentA(number_to_display) !=0)
				SET_DISPLAY(SegmentA(number_to_display));
			else 
				CLEAR_DISPLAY;
			display_status.segmentState += screen_toggle_var[screen_toggle_rate];
		break;

		case RIGHT: //Segment B
			CLEAR_DISPLAY;
			RESET_ANODE1;
			LEDToggle();
			SET_ANODE2;
			
			SET_DISPLAY(SegmentB(number_to_display));		
			display_status.segmentState += screen_toggle_var[screen_toggle_rate];
		break;

		case BLANK:
			CLEAR_DISPLAY;
			RESET_ANODE1;
			RESET_ANODE2;
			display_status.segmentState += screen_toggle_var[screen_toggle_rate];
			LED_3_OFF;
		break;
	}
}

void displayCharSegment(uint16_t char_A_to_display, uint16_t char_B_to_display)
{
	
	if (display_status.segmentState >= MAX_NUMBER_DISPLAY_BLANKS)
				display_status.segmentState = 0;
	
	switch (display_segment_switch[display_status.segmentState])
	{
		case LEFT: //Segment A

			CLEAR_DISPLAY;
			RESET_ANODE2;
			redLed();
			SET_ANODE1;
			SET_DISPLAY(char_A_to_display);
			display_status.segmentState += screen_toggle_var[screen_toggle_rate];
		break;

		case RIGHT: //Segment B
			CLEAR_DISPLAY;
			RESET_ANODE1;
			LEDToggle();
			SET_ANODE2;
			SET_DISPLAY(char_B_to_display);		
			display_status.segmentState += screen_toggle_var[screen_toggle_rate];
		break;

		case BLANK:
			CLEAR_DISPLAY;
			RESET_ANODE1;
			RESET_ANODE2;
			display_status.segmentState += screen_toggle_var[screen_toggle_rate];
			LED_3_OFF;
		break;
	}
}

void displayBlinking(uint16_t number_to_display)
{
	static uint32_t blinkOn = 0;
	if(blinkOn < BLINKING_INTERVAL)
		displayNumberSegment(display_status.currentTarget);
	else if (blinkOn < BLINKING_INTERVAL * 2)
		CLEAR_DISPLAY;
	else
		blinkOn = 0;
	
	blinkOn++;
}

void displayTripleBlinker(uint16_t blinking_message)
{
	CLEAR_DISPLAY;
	SET_ANODE1;
	SET_ANODE2;
	for(uint16_t i = 0; i < 3; i++)
	{
		uint32_t blinkCyclade = 0; 
		for(blinkCyclade = 0; blinkCyclade < TRIPLE_BLINKING_SCREEN; blinkCyclade++)
			SET_DISPLAY(blinking_message);
		for(blinkCyclade = 0; blinkCyclade < TRIPLE_BLINKING_SCREEN; blinkCyclade++)
			CLEAR_DISPLAY;
	}
}

void displayLockedScreen()
{
	CLEAR_DISPLAY;
	SET_ANODE1;
	SET_ANODE2;
	for(uint32_t i = 0; i < LOCKED_SCREEN_UPON_BUTTON; i++)
		SET_DISPLAY(16);
}


void programSelectDisplay()
{
	programDisplay();
}

void openWindowDisplay()
{
	if(display_status.openWindowState.selectedOpenWindowDisplay)
	{
		if(display_status.openWindowState.openWindowState)
			displayCharSegment(0, 25);
		else
			displayCharSegment(26, 14);
	}
	else
	displayCharSegment(0, 23);
}

void calibrationDisplay()
{
	if(calibration_active)
		displayNumberSegment(Calculated_temperature/10);
	else
		displayCharSegment(18, 12);
}

void adaptiveStartDisplay()
{
	if(adaptive_start_state.selectedAdaptiveStartDisplay)
	{
		if(adaptive_start_state.adaptiveStartStatus)
			displayCharSegment(0, 25);
		else
			displayCharSegment(26, 14);
	}
	else
	displayCharSegment(24, 5);
}

void factorySettingsDisplay()
{
	if(factorySettingsActive)
	{
		FACTORY_DISPLAY(factorySettingsRound[factorySettingsSetRound]);
		factorySettingsDisplayTimer();
	}
	else
	{
		displayCharSegment(20, 21);
	}
}

void antifreezeDisplay()
{
	displayCharSegment(24, 14);
}

void display()
{
	switch(display_status.currentState)
	{
		case COMFORT:
			displayNumberSegment(display_status.currentTarget);
			break;


		case TIME_DATE:
			timeDateDisplay(&display_status);
			break;	


		case PROGRAM_SELECT:
			programSelectDisplay();
			break;	


		case OPEN_WINDOW:
			openWindowDisplay();
			break;	


		case LOCK:
			displayNumberSegment(display_status.currentTarget);
			break;


		case CALIBRATION:
			calibrationDisplay();
			break;


		case ADAPTIVE_START:
			adaptiveStartDisplay();
			break;


		case FACTORY_SETTINGS:
			factorySettingsDisplay();
			break;


		case ANTIFREEZE:
			antifreezeDisplay();
			break;


		case OFF:
			displayCharSegment(19, 19);
			break;


		case POWER_LOSS:
			programsCheckForLowestTarget();
			displayBlinking(display_status.currentTarget);
			break;
		
		case HARDWARE_TEST:
			if (display_status.currentTarget != 88)
				CLEAR_DISPLAY;
			else
				displayNumberSegment(display_status.currentTarget);
			break;
			
		case PROGRAM_MENU:
			programMenuSelect();
			break;
	}
}

