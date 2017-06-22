#ifndef __led_H
#define __led_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdbool.h>

extern bool doBlinkingLED;
//extern uint16_t LEDblinkingCounter;


void LEDToggle(void);
void redLed(void);
void LEDBlinkingcount(void);
void LEDSetBlinking(void);

#ifdef __cplusplus
}
#endif
#endif

