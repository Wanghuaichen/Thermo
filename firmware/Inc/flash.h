#ifndef __flash_H
#define __flash_H
#ifdef __cplusplus
 extern "C" {
#endif

	 #include <stdbool.h>

	 #define MAIN_SECTOR  									0x08007C00
	 #define COMFORT_TEMPERATURE							0x00
	 #define TIME_DATE_WAS_SET								0x02
	 #define IS_OPEN_WINDOW									0x04
	 #define IS_ADAPTIVE_START								0x06
	 #define CURRENT_PROGRAM								0x08
	 #define PROGRAM_1_WEEKDAY_START_HOUR					0X0A
	 #define PROGRAM_1_WEEKDAY_START_MINUTE					0X0C
	 #define PROGRAM_1_WEEKDAY_END_HOUR						0X0E
	 #define PROGRAM_1_WEEKDAY_END_MINUTE					0X10
	 #define PROGRAM_1_WEEKDAY_TEMPERATURE					0X12
	 #define PROGRAM_1_WEEKDAY_HEATING_IS_ON				0X14
	 #define PROGRAM_2_WEEKDAY_START_HOUR					0x16
	 #define PROGRAM_2_WEEKDAY_START_MINUTE					0x18
	 #define PROGRAM_2_WEEKDAY_END_HOUR						0X1A
	 #define PROGRAM_2_WEEKDAY_END_MINUTE					0x1C
	 #define PROGRAM_2_WEEKDAY_TEMPERATURE					0x1E
	 #define PROGRAM_2_WEEKDAY_HEATING_IS_ON				0x20
	 #define PROGRAM_3_WEEKDAY_START_HOUR					0x22
	 #define PROGRAM_3_WEEKDAY_START_MINUTE					0X24
	 #define PROGRAM_3_WEEKDAY_END_HOUR						0x26
	 #define PROGRAM_3_WEEKDAY_END_MINUTE					0x28
	 #define PROGRAM_3_WEEKDAY_TEMPERATURE					0x2A
	 #define PROGRAM_3_WEEKDAY_HEATING_IS_ON				0x2C
	 #define CALIBRATION_VALUE								0x2E
	 #define PROGRAM_1_WEEKEND_START_HOUR					0X30
	 #define PROGRAM_1_WEEKEND_START_MINUTE					0X32
	 #define PROGRAM_1_WEEKEND_END_HOUR						0X34
	 #define PROGRAM_1_WEEKEND_END_MINUTE					0X36
	 #define PROGRAM_1_WEEKEND_TEMPERATURE					0X38
	 #define PROGRAM_1_WEEKEND_HEATING_IS_ON				0X3A
	 #define PROGRAM_2_WEEKEND_START_HOUR					0x3C
	 #define PROGRAM_2_WEEKEND_START_MINUTE					0x3E
	 #define PROGRAM_2_WEEKEND_END_HOUR						0X40
	 #define PROGRAM_2_WEEKEND_END_MINUTE					0x42
	 #define PROGRAM_2_WEEKEND_TEMPERATURE					0x44
	 #define PROGRAM_2_WEEKEND_HEATING_IS_ON				0x46
	 #define PROGRAM_3_WEEKEND_START_HOUR					0x48
	 #define PROGRAM_3_WEEKEND_START_MINUTE					0X4A
	 #define PROGRAM_3_WEEKEND_END_HOUR						0x4C
	 #define PROGRAM_3_WEEKEND_END_MINUTE					0x4E
	 #define PROGRAM_3_WEEKEND_TEMPERATURE					0x50
	 #define PROGRAM_3_WEEKEND_HEATING_IS_ON				0x52
	 #define FLASH_READ_FLAG								0x54
	 
	 
	void flashWriteHalfword(void);
	void flashReadHalfword(void);
	bool wasFlashSet(void);
	void flashFullErase(void);
	 
	 




#ifdef __cplusplus
}
#endif
#endif
