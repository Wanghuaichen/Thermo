#include "adaptive_start.h"
#include "temperature.h"
#include "adc.h"
#include <math.h>

//#define	MAX_ADC_VALUES_ARRAY 	25
#define MAX_ADC_VALUE			4096
#define T_MID_TEMP				273
#define T_CONSTANT 				298
#define B_CONSTANT				3970
#define R2						10000
#define CHIP_VOLTAGE			3300
#define ADC_VALUE_INVALID		0xFFFF
#define ALPHA_CONSTANT			0.8


//uint16_t adc_value_array[MAX_ADC_VALUES_ARRAY];
int16_t Calculated_temperature = 0;
int16_t Calibration_Value = 0;
bool calibration_active = false;
bool getTemperatureVal = false;



uint16_t averageValueCalcADC()
{
	static uint16_t adc_value = 0;
	/* for average
	static uint16_t i = 0;
	if (i == MAX_ADC_VALUES_ARRAY)
		i = 0;
	adc_value = adc_value - adc_value_array[i];
	adc_value_array[i] = adcGetADCData()/MAX_ADC_VALUES_ARRAY;
		if(adc_value_array[i] == 0)
			return ADC_VALUE_INVALID;
	adc_value = adc_value + adc_value_array[i];
	i++;
	*/
	adc_value = adcGetADCData();
	if(adc_value == 0)
		return ADC_VALUE_INVALID;
	
	return adc_value; 
}


uint16_t adcGetVoltage()
{
	uint16_t average_adc_value = averageValueCalcADC();
		if(average_adc_value == ADC_VALUE_INVALID)
			return ADC_VALUE_INVALID;
	uint16_t voltage_value = average_adc_value * CHIP_VOLTAGE / MAX_ADC_VALUE;
	
	return voltage_value;
}

uint32_t adcGetResistance()
{
	uint16_t voltage_value = adcGetVoltage();
	if(voltage_value == ADC_VALUE_INVALID)
		return ADC_VALUE_INVALID;
	uint32_t r_term = ((R2 * voltage_value) / (CHIP_VOLTAGE - voltage_value));
	
	return r_term;
}

uint16_t adcGetTemperature()
{
	double resistance_value = adcGetResistance();
		if(resistance_value == ADC_VALUE_INVALID)
			return ADC_VALUE_INVALID;
	double temperature_value = ((double)1 / (((log(resistance_value/(double)R2))/(double)B_CONSTANT) + ((double)1/(double)T_CONSTANT))) - T_MID_TEMP;
	
	temperature_value = (ALPHA_CONSTANT * (Calculated_temperature/(double)10)) + ((1-ALPHA_CONSTANT) * temperature_value); 
	
 	return temperature_value * 10;
}

void updateTemperature()
{
	if(getTemperatureVal)
	{
		Calculated_temperature = adcGetTemperature() + (Calibration_Value * 3);
		getTemperatureVal = false;
	}
}


void IncrementCalibrationValue()
{
	if(calibration_active)
	{
		Calibration_Value++;
		if(Calibration_Value > 5)
			Calibration_Value = 5;
	}
}

void DecrementCalibrationValue()
{
	if(calibration_active)
	{
		Calibration_Value--;
		if(Calibration_Value < -5)
			Calibration_Value = -5;
	}
}

void SwitchCalibrationState()
{
	if(calibration_active)
		calibration_active = false;
	else
		calibration_active = true;
}


