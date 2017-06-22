#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_flash.h"


#include "current_state.h"
#include "programs.h"
#include "temperature.h"
#include "flash.h"


#define ACDC	0xACDC







void flashFullErase()
{
	HAL_FLASH_Unlock();
	
	FLASH_PageErase(MAIN_SECTOR);
	
	HAL_FLASH_Lock();
	
}


/* Write to memory*/
void flashWriteHalfword(void)
{
	HAL_FLASH_Unlock();
	
	FLASH_PageErase(MAIN_SECTOR);
	
	// Comfort and current states 
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|COMFORT_TEMPERATURE), current_status.Temperature.comfort);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|TIME_DATE_WAS_SET), current_status.timeDateWasSet);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|IS_OPEN_WINDOW), current_status.isOpenWindow);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|IS_ADAPTIVE_START), current_status.isAdaptiveStart);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|CURRENT_PROGRAM), current_status.currentProgram);
	
	
	// Programs schedules 
	// first 
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_1_WEEKDAY_START_HOUR), programs.program_1.week_day.start_hour);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_1_WEEKDAY_START_MINUTE), programs.program_1.week_day.start_minute);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_1_WEEKDAY_END_HOUR), programs.program_1.week_day.end_hour);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_1_WEEKDAY_END_MINUTE), programs.program_1.week_day.end_minute);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_1_WEEKDAY_TEMPERATURE), programs.program_1.week_day.target_celsius);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_1_WEEKDAY_HEATING_IS_ON), programs.program_1.week_day.isProgramOff);
	
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_1_WEEKEND_START_HOUR), programs.program_1.week_end.start_hour);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_1_WEEKEND_START_MINUTE), programs.program_1.week_end.start_minute);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_1_WEEKEND_END_HOUR), programs.program_1.week_end.end_hour);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_1_WEEKEND_END_MINUTE), programs.program_1.week_end.end_minute);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_1_WEEKEND_TEMPERATURE), programs.program_1.week_end.target_celsius);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_1_WEEKEND_HEATING_IS_ON), programs.program_1.week_end.isProgramOff);

	// second
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_2_WEEKDAY_START_HOUR), programs.program_2.week_day.start_hour);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_2_WEEKDAY_START_MINUTE), programs.program_2.week_day.start_minute);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_2_WEEKDAY_END_HOUR), programs.program_2.week_day.end_hour);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_2_WEEKDAY_END_MINUTE), programs.program_2.week_day.end_minute);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_2_WEEKDAY_TEMPERATURE), programs.program_2.week_day.target_celsius);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_2_WEEKDAY_HEATING_IS_ON), programs.program_2.week_day.isProgramOff);
	
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_2_WEEKEND_START_HOUR), programs.program_2.week_end.start_hour);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_2_WEEKEND_START_MINUTE), programs.program_2.week_end.start_minute);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_2_WEEKEND_END_HOUR), programs.program_2.week_end.end_hour);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_2_WEEKEND_END_MINUTE), programs.program_2.week_end.end_minute);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_2_WEEKEND_TEMPERATURE), programs.program_2.week_end.target_celsius);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_2_WEEKEND_HEATING_IS_ON), programs.program_2.week_end.isProgramOff);
	
	// third 
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_3_WEEKDAY_START_HOUR), programs.program_3.week_day.start_hour);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_3_WEEKDAY_START_MINUTE), programs.program_3.week_day.start_minute);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_3_WEEKDAY_END_HOUR), programs.program_3.week_day.end_hour);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_3_WEEKDAY_END_MINUTE), programs.program_3.week_day.end_minute);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_3_WEEKDAY_TEMPERATURE), programs.program_3.week_day.target_celsius);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_3_WEEKDAY_HEATING_IS_ON), programs.program_3.week_day.isProgramOff);

	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_3_WEEKEND_START_HOUR), programs.program_3.week_end.start_hour);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_3_WEEKEND_START_MINUTE), programs.program_3.week_end.start_minute);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_3_WEEKEND_END_HOUR), programs.program_3.week_end.end_hour);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_3_WEEKEND_END_MINUTE), programs.program_3.week_end.end_minute);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_3_WEEKEND_TEMPERATURE), programs.program_3.week_end.target_celsius);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|PROGRAM_3_WEEKEND_HEATING_IS_ON), programs.program_3.week_end.isProgramOff);

	// Calibration
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|CALIBRATION_VALUE), Calibration_Value);
	
	// Dude, where is the ACDC CD? 
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (MAIN_SECTOR|FLASH_READ_FLAG), ACDC);

	
	//
//	Lock flash write 
	HAL_FLASH_Lock();
}


bool wasFlashSet()
{
	uint16_t readOrInit;
	readOrInit= *(unsigned short*)(MAIN_SECTOR|FLASH_READ_FLAG);
	if (readOrInit == ACDC)
		return true;
	else
		return false;
}

void flashReadHalfword(void)
{
	
	current_status.Temperature.comfort 					= *(unsigned short*)(MAIN_SECTOR|COMFORT);
	current_status.timeDateWasSet 							= *(unsigned short*)(MAIN_SECTOR|TIME_DATE_WAS_SET);
	current_status.isOpenWindow 								= *(unsigned short*)(MAIN_SECTOR|IS_OPEN_WINDOW);
	current_status.isAdaptiveStart 							= *(unsigned short*)(MAIN_SECTOR|IS_ADAPTIVE_START);
	current_status.currentProgram 							= *(program_menu_t*)(MAIN_SECTOR|CURRENT_PROGRAM);
	
	
	/** first **/
	programs.program_1.week_day.start_hour			= *(unsigned short*)(MAIN_SECTOR|PROGRAM_1_WEEKDAY_START_HOUR);
	programs.program_1.week_day.start_minute		= *(unsigned short*)(MAIN_SECTOR|PROGRAM_1_WEEKDAY_START_MINUTE);
	programs.program_1.week_day.end_hour				=	*(unsigned short*)(MAIN_SECTOR|PROGRAM_1_WEEKDAY_END_HOUR);
	programs.program_1.week_day.end_minute			= *(unsigned short*)(MAIN_SECTOR|PROGRAM_1_WEEKDAY_END_MINUTE);
	programs.program_1.week_day.target_celsius	=	*(unsigned short*)(MAIN_SECTOR|PROGRAM_1_WEEKDAY_TEMPERATURE);
	programs.program_1.week_day.isProgramOff		= *(unsigned short*)(MAIN_SECTOR|PROGRAM_1_WEEKDAY_HEATING_IS_ON);

	programs.program_1.week_end.start_hour			= *(unsigned short*)(MAIN_SECTOR|PROGRAM_1_WEEKEND_START_HOUR);
	programs.program_1.week_end.start_minute  	= *(unsigned short*)(MAIN_SECTOR|PROGRAM_1_WEEKEND_START_MINUTE);
	programs.program_1.week_end.end_hour				= *(unsigned short*)(MAIN_SECTOR|PROGRAM_1_WEEKEND_END_HOUR);
	programs.program_1.week_end.end_minute 			=	*(unsigned short*)(MAIN_SECTOR|PROGRAM_1_WEEKEND_END_MINUTE);
	programs.program_1.week_end.target_celsius	= *(unsigned short*)(MAIN_SECTOR|PROGRAM_1_WEEKEND_TEMPERATURE);
	programs.program_1.week_end.isProgramOff		= *(unsigned short*)(MAIN_SECTOR|PROGRAM_1_WEEKEND_HEATING_IS_ON);
	
	/** second **/
	programs.program_2.week_day.start_hour			= *(unsigned short*)(MAIN_SECTOR|PROGRAM_2_WEEKDAY_START_HOUR);
	programs.program_2.week_day.start_minute		= *(unsigned short*)(MAIN_SECTOR|PROGRAM_2_WEEKDAY_START_MINUTE);
	programs.program_2.week_day.end_hour				=	*(unsigned short*)(MAIN_SECTOR|PROGRAM_2_WEEKDAY_END_HOUR);
	programs.program_2.week_day.end_minute			= *(unsigned short*)(MAIN_SECTOR|PROGRAM_2_WEEKDAY_END_MINUTE);
	programs.program_2.week_day.target_celsius	=	*(unsigned short*)(MAIN_SECTOR|PROGRAM_2_WEEKDAY_TEMPERATURE);
	programs.program_2.week_day.isProgramOff		= *(unsigned short*)(MAIN_SECTOR|PROGRAM_2_WEEKDAY_HEATING_IS_ON);

	programs.program_2.week_end.start_hour			= *(unsigned short*)(MAIN_SECTOR|PROGRAM_2_WEEKEND_START_HOUR);
	programs.program_2.week_end.start_minute  	= *(unsigned short*)(MAIN_SECTOR|PROGRAM_2_WEEKEND_START_MINUTE);
	programs.program_2.week_end.end_hour				= *(unsigned short*)(MAIN_SECTOR|PROGRAM_2_WEEKEND_END_HOUR);
	programs.program_2.week_end.end_minute 			=	*(unsigned short*)(MAIN_SECTOR|PROGRAM_2_WEEKEND_END_MINUTE);
	programs.program_2.week_end.target_celsius	= *(unsigned short*)(MAIN_SECTOR|PROGRAM_2_WEEKEND_TEMPERATURE);
	programs.program_2.week_end.isProgramOff		= *(unsigned short*)(MAIN_SECTOR|PROGRAM_2_WEEKEND_HEATING_IS_ON);
	
	/** third **/
	programs.program_3.week_day.start_hour			= *(unsigned short*)(MAIN_SECTOR|PROGRAM_3_WEEKDAY_START_HOUR);
	programs.program_3.week_day.start_minute		= *(unsigned short*)(MAIN_SECTOR|PROGRAM_3_WEEKDAY_START_MINUTE);
	programs.program_3.week_day.end_hour				=	*(unsigned short*)(MAIN_SECTOR|PROGRAM_3_WEEKDAY_END_HOUR);
	programs.program_3.week_day.end_minute			= *(unsigned short*)(MAIN_SECTOR|PROGRAM_3_WEEKDAY_END_MINUTE);
	programs.program_3.week_day.target_celsius	=	*(unsigned short*)(MAIN_SECTOR|PROGRAM_3_WEEKDAY_TEMPERATURE);
	programs.program_3.week_day.isProgramOff		= *(unsigned short*)(MAIN_SECTOR|PROGRAM_3_WEEKDAY_HEATING_IS_ON);

	programs.program_3.week_end.start_hour			= *(unsigned short*)(MAIN_SECTOR|PROGRAM_3_WEEKEND_START_HOUR);
	programs.program_3.week_end.start_minute  	= *(unsigned short*)(MAIN_SECTOR|PROGRAM_3_WEEKEND_START_MINUTE);
	programs.program_3.week_end.end_hour				= *(unsigned short*)(MAIN_SECTOR|PROGRAM_3_WEEKEND_END_HOUR);
	programs.program_3.week_end.end_minute 			=	*(unsigned short*)(MAIN_SECTOR|PROGRAM_3_WEEKEND_END_MINUTE);
	programs.program_3.week_end.target_celsius	= *(unsigned short*)(MAIN_SECTOR|PROGRAM_3_WEEKEND_TEMPERATURE);
	programs.program_3.week_end.isProgramOff		= *(unsigned short*)(MAIN_SECTOR|PROGRAM_3_WEEKEND_HEATING_IS_ON);
	
	/** calibration **/
	Calibration_Value														= *(unsigned short*)(MAIN_SECTOR|CALIBRATION_VALUE);
	
	/** Flash or read **/
	
	
}




