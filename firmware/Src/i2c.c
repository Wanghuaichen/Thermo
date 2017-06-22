/**
  ******************************************************************************
  * File Name          : I2C.c
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */

#include "schedules.h"
#include "display.h"

#define DECIMAL_TO_BCD(dec) ((((dec / 10) << 4) & 0xF0) + (dec % 10))



typedef struct starting_rtc_data
{
	uint8_t Data_seconds[2];// 	// Sekundes registras, reiksme
	uint8_t	Data_minutes[2];// 	// Minutes registras, reiksme		
	uint8_t		Data_hours[2];// 	// Valandos registras, reiksme
	uint8_t			Data_Day[2];// 	// Savaites dienos registras, reiksme
}	starting_rtc_time_t;


rtc_time_active_t RTC_Time;
//uint8_t RTC_array[5];
starting_rtc_time_t rtc_time_addressing;

uint8_t SLVadd;
/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */
  
    /**I2C1 GPIO Configuration    
    PA9     ------> I2C1_SCL
    PA10     ------> I2C1_SDA 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PA9     ------> I2C1_SCL
    PA10     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  }
  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
}

/* USER CODE BEGIN 1 */


bool i2cTestForRTCChip()
{
	uint8_t err_code;
	
	err_code = HAL_I2C_Master_Transmit(&hi2c1, 0xD0, rtc_time_addressing.Data_seconds, 2, 100);
	if(err_code == 0x00)
	{
		SLVadd = 0xD0;
		return err_code;
	}
	
	err_code = HAL_I2C_Master_Transmit(&hi2c1, 0xA2, rtc_time_addressing.Data_seconds, 2, 100);
	if(err_code == 0x00)
	{
		SLVadd = 0xA2;
		return err_code;
	}
	
	return err_code;
}
		//TODO set on init the registers that make RTC 0 when no power
//TODO when on program, comfo temp switches back to its original temp on rtc read
uint8_t i2cReturnSlaveAddress()
{
	return SLVadd;
}

void setup_rtc_addresses()
{
	if(SLVadd == 0xD0)
	{
		rtc_time_addressing.Data_seconds[0] = 0x00; // address
		rtc_time_addressing.Data_seconds[1] = 0x01;	// value
		
		rtc_time_addressing.Data_minutes[0] = 0x01;	// address
		rtc_time_addressing.Data_minutes[1] = 0x15; // value
		
		rtc_time_addressing.Data_hours[0] 	= 0x02;	// address
		rtc_time_addressing.Data_hours[1] 	= 0x16;  // value
		
		rtc_time_addressing.Data_Day[0] 		= 0x03;	// address
		rtc_time_addressing.Data_Day[1] 		= 0x02;  // value
		
	}
	else if(SLVadd == 0xA2)
	{
		rtc_time_addressing.Data_seconds[0] = 0x02; // address
		rtc_time_addressing.Data_seconds[1] = 0x01;	// value
		
		rtc_time_addressing.Data_minutes[0] = 0x03;	// address
		rtc_time_addressing.Data_minutes[1] = 0x15; // value
		
		rtc_time_addressing.Data_hours[0] 	= 0x04;	// address
		rtc_time_addressing.Data_hours[1] 	= 0x16;  // value
		
		rtc_time_addressing.Data_Day[0] 		= 0x06;	// address
		rtc_time_addressing.Data_Day[1] 		= 0x02;  // value
	}
}


void rtcSetTimeToRTC()
{
		HAL_I2C_Master_Transmit(&hi2c1, SLVadd, rtc_time_addressing.Data_seconds, 2, 100);		
		HAL_I2C_Master_Transmit(&hi2c1, SLVadd, rtc_time_addressing.Data_minutes, 2, 100);		
		HAL_I2C_Master_Transmit(&hi2c1, SLVadd, rtc_time_addressing.Data_hours, 2, 100);			
		HAL_I2C_Master_Transmit(&hi2c1, SLVadd, rtc_time_addressing.Data_Day, 2, 100);
	
	i2cGetRTCValues();
}

bool checkForPowerLoss()
{
	if((RTC_Time.Hours == 0x00) && (RTC_Time.Minutes == 0x00))
		return true;
	else
		return false;
}

void i2cRTCstartingTimeInit(bool setDefaultTime)
{
	if(i2cTestForRTCChip() == 0x00)
	{
		//this is good
	}
	setup_rtc_addresses();
	if(setDefaultTime)
	{
		rtcSetTimeToRTC();
	}
	else
	{
		i2cGetRTCValues();
		if(checkForPowerLoss())
		{
			display_status.currentState = POWER_LOSS;
		}
	}

}

void i2cWriteTimeToRTC(uint8_t Minutes, uint8_t Hours, uint8_t Day)
{
	rtc_time_addressing.Data_minutes[1] = DECIMAL_TO_BCD(Minutes); 
	rtc_time_addressing.Data_hours[1] 	= DECIMAL_TO_BCD(Hours);  	
	rtc_time_addressing.Data_Day[1] 		= DECIMAL_TO_BCD(Day);  		

	if(SLVadd == 0xA2)
		if(rtc_time_addressing.Data_Day[1] == 7) rtc_time_addressing.Data_Day[1] = 0;
	
	rtcSetTimeToRTC();
	
}

void RTCHexToDec(uint8_t *time_Array)
{
	if(SLVadd == 0xA2)
	{
		RTC_Time.Seconds = ((time_Array[0] & 0x70)/0x10)*10 + (time_Array[0] % 0x10);
		RTC_Time.Minutes = ((time_Array[1] & 0x70)/0x10)*10 + (time_Array[1] % 0x10);
		RTC_Time.Hours 	 = ((time_Array[2] & 0x30)/0x10)*10 + (time_Array[2] % 0x10);
		RTC_Time.Week_Day =((time_Array[4] & 0x07));
		if (RTC_Time.Week_Day == 0) RTC_Time.Week_Day = 7;
	}
	if(SLVadd == 0xD0)
	{
		RTC_Time.Seconds = ((time_Array[0] & 0x70)/0x10)*10 + (time_Array[0] % 0x10);
		RTC_Time.Minutes = ((time_Array[1] & 0x70)/0x10)*10 + (time_Array[1] % 0x10);
		RTC_Time.Hours 	 = ((time_Array[2] & 0x30)/0x10)*10 + (time_Array[2] % 0x10);
		RTC_Time.Week_Day =((time_Array[3] & 0x07));
	}
}

void i2cGetRTCValues(void)
{
		uint8_t RTC_array[5];

		HAL_I2C_Master_Transmit(&hi2c1, SLVadd,			 rtc_time_addressing.Data_seconds,				1, 100);
	
		HAL_I2C_Master_Receive(&hi2c1,  SLVadd|0x01, RTC_array, 															5, 100);
	
		RTCHexToDec(RTC_array);
	
		schedulesCheck();
}

void passRTCValues(rtc_time_active_t* get_value)
{
	*get_value = RTC_Time;
}

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
