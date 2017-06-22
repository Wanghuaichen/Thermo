#ifndef __macros_H
#define __macros_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "gpio.h"




#define SET_ANODE1 HAL_GPIO_WritePin(GPIOA, ANODE1_Pin, GPIO_PIN_RESET)
#define SET_ANODE2 HAL_GPIO_WritePin(GPIOA, ANODE2_Pin, GPIO_PIN_RESET)
#define RESET_ANODE1 HAL_GPIO_WritePin(GPIOA, ANODE1_Pin, GPIO_PIN_SET)
#define RESET_ANODE2 HAL_GPIO_WritePin(GPIOA, ANODE2_Pin, GPIO_PIN_SET)

#define CLEAR_DISPLAY HAL_GPIO_WritePin(GPIOB, 0xFF, GPIO_PIN_RESET)
#define SET_DISPLAY(number) HAL_GPIO_WritePin(GPIOB, segments[number], GPIO_PIN_SET)
#define FACTORY_DISPLAY(number) HAL_GPIO_WritePin(GPIOB, number, GPIO_PIN_SET)

#define BUTTON_5_IS_PRESSED (HAL_GPIO_ReadPin(GPIOA, SW5_Pin) == 0)
#define BUTTON_4_IS_PRESSED (HAL_GPIO_ReadPin(GPIOA, SW4_Pin) == 0)
#define BUTTON_3_IS_PRESSED (HAL_GPIO_ReadPin(GPIOA, SW3_Pin) == 0)
#define BUTTON_2_IS_PRESSED (HAL_GPIO_ReadPin(GPIOA, SW2_Pin) == 0)
#define BUTTON_1_IS_PRESSED (HAL_GPIO_ReadPin(GPIOA, SW1_Pin) == 0)

#define SET_TRIAC		HAL_GPIO_WritePin(GPIOA, TRIAC_Pin, GPIO_PIN_SET)
#define RESET_TRIAC		HAL_GPIO_WritePin(GPIOA, TRIAC_Pin, GPIO_PIN_RESET)

#define LED_1_2_ON		HAL_GPIO_WritePin(GPIOA, LED_1_2_Pin, GPIO_PIN_RESET)
#define LED_1_2_OFF		HAL_GPIO_WritePin(GPIOA, LED_1_2_Pin, GPIO_PIN_SET)
#define LED_3_ON		HAL_GPIO_WritePin(GPIOA, LED_3_Pin, GPIO_PIN_RESET)
#define LED_3_OFF 		HAL_GPIO_WritePin(GPIOA, LED_3_Pin, GPIO_PIN_SET)

#ifdef __cplusplus
}
#endif
#endif

