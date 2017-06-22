
#ifndef __time_date_H
#define __time_date_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "display.h"

typedef enum weekDay
{
	MONDAY = 1,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	SUNDAY
} week_days_t;

typedef struct time_date_info
{
	week_days_t day;
	uint16_t hour;
	uint16_t minute;
} time_date_info_t;

extern time_date_info_t time_date_info;

void timeDateInit(void);
void timeDateDisplay(display_status_t *display_status);
void timeDateButtonHandlerIncrement(void);
void timeDateButtonHandlerDecrement(void);
void timeDateButtonState(void);

#ifdef __cplusplus
}
#endif
#endif


