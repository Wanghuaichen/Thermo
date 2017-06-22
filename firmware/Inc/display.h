#ifndef __display_H
#define __display_H
#ifdef __cplusplus
 extern "C" {
#endif


#include	"stdint.h"
#include	"main.h"
#include	"gpio.h"
#include	"macros.h"

extern uint16_t segments[];


typedef enum segment_side
{
	BLANK,
	LEFT,
	RIGHT
} segment_state_t;


typedef enum active_state
{
	COMFORT,
	TIME_DATE,
	PROGRAM_SELECT,
	OPEN_WINDOW,
	CALIBRATION,
	ADAPTIVE_START,
	FACTORY_SETTINGS,
	LOCK,
	ANTIFREEZE,
	OFF,
	POWER_LOSS,
	HARDWARE_TEST,
	PROGRAM_MENU
} active_state_t;



typedef enum timeDate
{	
	TIME_DATE_MAIN,
	TIME_DATE_WEEK_DAY,
	TIME_DATE_HOUR,
	TIME_DATE_MINUTE,
	TIME_DATE_END
} time_date_t;


typedef struct open_window_status
{
	bool openWindowState;
	bool selectedOpenWindowDisplay;
} open_window_state_t;


typedef struct tempStates
{
	uint16_t comfort;
	uint16_t program_temperature;
} temperature_state_t;

extern temperature_state_t tempProgramState;



typedef struct toDisplay
{
	uint16_t 				currentTarget;
	uint16_t				segmentState;
	active_state_t			currentState;
	time_date_t 			currentTimeDateState;
	open_window_state_t		openWindowState;
} display_status_t;




extern display_status_t	display_status;



void display(void);
void display_init(void);
bool displayDecrementScreenRate(void);
bool displayActivateScreen(void);
void displayLockedScreen(void);
void displayTripleBlinker(uint16_t blinking_message);
void displayCharSegment(uint16_t char_A_to_display, uint16_t char_B_to_display);
void displayNumberSegment(uint16_t number_to_display);



#ifdef __cplusplus
}
#endif
#endif


