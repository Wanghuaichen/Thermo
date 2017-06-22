#ifndef __programs_H
#define __programs_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
	 
typedef enum programSelect
{
	PROGRAM_1 = 1,
	PROGRAM_2,
	PROGRAM_3,
} program_select_t;

typedef enum programType
{
	WEEKDAY,
	WEEKEND,
} program_type_t;

typedef enum programSettings
{
	NO_PROGRAM,
	START,
	START_HOUR,
	START_MINUTE,
	END,
	END_HOUR,
	END_MINUTE,
	CELSIUS,
	TEMPERATURE,
} program_settings_t;

typedef struct programVariables
{
	uint8_t start_hour;
	uint8_t start_minute;
	uint8_t end_hour;
	uint8_t end_minute;
	uint8_t target_celsius;
	bool 		isProgramOff;
} program_variables_t;

typedef struct programDayType
{
	program_variables_t week_day;
	program_variables_t week_end;	
} program_day_type_t;

typedef struct programs
{
	uint8_t programActive;
	program_day_type_t 	program_1;
	program_day_type_t 	program_2;
	program_day_type_t 	program_3;
	program_select_t 	program_select;
	program_type_t 		program_type;
	program_settings_t 	program_settings;

} programs_t;

extern programs_t programs;





void programsInit(bool doSetDefault);
void programDisplay(void);
void programActiveState(void);
void nextProgram(void);
void previousProgram(void);
void programPlus(void);
void programMinus(void);
void programsPassAlongSettings(void);
void settingProgramsButtonIncrease(void);
void settingProgramsButtonDecrease(void);
void programsCheckForLowestTarget(void);

#ifdef __cplusplus
}
#endif
#endif
