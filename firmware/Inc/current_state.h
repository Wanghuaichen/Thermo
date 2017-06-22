#ifndef __current_state_H
#define __current_state_H
#ifdef __cplusplus
 extern "C" {
#endif

	 
#include "time_date.h"
#include "program_menu.h"
#include "programs.h"
	 
	 
typedef struct status
{
	
	temperature_state_t Temperature;
	time_date_info_t 		CurrentTimeDate;
	bool								timeDateWasSet;
	bool 								isOpenWindow;
	bool								isAdaptiveStart;
	program_menu_t			currentProgram;
	program_day_type_t	currentSchedule;
	bool isSchedulePerformingWeekDay;
	bool isSchedulePerformingWeekEnd;
	bool heatingIsON;
	
} current_status_t;

extern current_status_t current_status;


#ifdef __cplusplus
}
#endif
#endif


