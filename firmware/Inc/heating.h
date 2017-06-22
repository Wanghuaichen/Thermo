
#ifndef __heating_H
#define __heating_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern bool triac;
extern bool DutyCylePercentage;
	 
	 
void heatingSetTargetTemperature(int16_t temperature_to_targeted);

#ifdef __cplusplus
}
#endif
#endif
