#ifndef __temperature_H
#define __temperature_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern int16_t Calculated_temperature;
extern int16_t Calibration_Value;
extern bool calibration_active;
extern bool getTemperatureVal;

	 
void updateTemperature(void);
void IncrementCalibrationValue(void);
void DecrementCalibrationValue(void);
void SwitchCalibrationState(void);
void temperature_init(void);
	 
#ifdef __cplusplus
}
#endif
#endif


