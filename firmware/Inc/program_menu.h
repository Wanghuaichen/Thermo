#ifndef __program_menu_H
#define __program_menu_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "main.h"


typedef enum programs_menu
{
	EMPTY_SPACE,
	PROGRAM_1_SELECT,
	PROGRAM_2_SELECT,
	PROGRAM_3_SELECT,
	OFF_SELECT,
	CANCEL_PROGRAM_SELECT,
	OVER_THE_LINE,
} program_menu_t;

extern program_menu_t programs_menu;



void programMenuSelect(void);
void programMenuDecrease(void);
void programMenuIncrease(void);
void programMenuSetComfort(void);
void programMenuSetCurrentProgram(void);



#ifdef __cplusplus
}
#endif
#endif

