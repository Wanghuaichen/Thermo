
#ifndef __adaptive_start_H
#define __adaptive_start_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "display.h"

typedef struct adaptive_start_status
{
	bool adaptiveStartStatus;
	bool selectedAdaptiveStartDisplay;
} adaptive_start_state_t;

extern adaptive_start_state_t adaptive_start_state;
extern bool startAdaptiveStart;
extern uint16_t adaptive_start_temp;


void adaptiveStartStateSelect(void);
void adaptiveStartStateDisplay(void);
void timeStampGet(void);
void adaptiveStartTimeCheck(void);


#ifdef __cplusplus
}
#endif
#endif


