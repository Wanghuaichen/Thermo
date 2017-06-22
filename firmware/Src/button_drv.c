#include "button_drv.h"
#include "stm32f0xx_hal.h"
#include "display.h"
#include "stm32f0xx_it.h"
#include "button_handler.h"




static volatile uint32_t pauseButtonUpdate = 0x00;
static volatile uint32_t pressedButtonsMask = 0x0;
static volatile uint32_t enabledButtonsMask = 0x0;
static volatile uint32_t longPushIssuedMask = 0x0;
static uint32_t registeredButtonsMask = 0x0;
//static uint32_t multiPushThreshold = 0;					// kolkas nenaudojamas - ISSIAISKINTI Paskirti
static void(*multi_button_cb)(void) = NULL;
typedef struct 
{
	uint32_t isEnabled;
	uint32_t pinNumber;
	uint32_t pinMask;
	uint32_t shortPushThreshold;
	uint32_t mediumPushThreshold;
	uint32_t longPushStartThreshold;
	volatile uint32_t currentState;
	volatile uint32_t pushTimeStamp;
	volatile uint32_t releaseTimeStamp;
	volatile push_event_t lastEvent;
	pushButtonCallback_t * pushButtonCallback;
} pushButtonConfig_t;

uint32_t PushButtonVariable = 0, Threshholder;
uint8_t alio = 0;


static pushButtonConfig_t bushButtonArray[NUM_OF_BUTTONS];

void  push_button_handler_cb(pushButtonConfig_t * pushButton);

static void  multi_button_cb_checker()//void * parg)
{
	if (NULL != multi_button_cb)
	{
		multi_button_cb();
	}
}

void  button_drv_init(void(*multi_push_cb)(void))
{

	multi_button_cb = multi_push_cb;
		
	uint32_t index;
	for (index = 0; index < NUM_OF_BUTTONS; index++)
	{
		bushButtonArray[index].pushButtonCallback = NULL;
	}
}


uint32_t  register_push_button(uint32_t pinNumber,
	uint32_t shortPushThreshold,
	uint32_t mediumPushThreshold, 
	uint32_t longPushStartThreshold,
	pushButtonCallback_t *pushButtonCallback)
{	
	uint32_t index;
	
	//Update existing push button
	for (index = 0; index < NUM_OF_BUTTONS; index++)
	{
		if ((bushButtonArray[index].pushButtonCallback != NULL) && (bushButtonArray[index].pinNumber == pinNumber))
		{
			bushButtonArray[index].pushButtonCallback = NULL;
			bushButtonArray[index].pushTimeStamp = 0;
			bushButtonArray[index].longPushStartThreshold = longPushStartThreshold*10;
			bushButtonArray[index].mediumPushThreshold = mediumPushThreshold*10;
			bushButtonArray[index].shortPushThreshold = shortPushThreshold*10;
			bushButtonArray[index].pushButtonCallback = pushButtonCallback;
			bushButtonArray[index].pushButtonCallback = pushButtonCallback;
			return true;
		}
	}
	
	//Add new push button
	for (index = 0; index < NUM_OF_BUTTONS; index++)
	{
		if (NULL == bushButtonArray[index].pushButtonCallback)
		{
			bushButtonArray[index].pinNumber = pinNumber;
			bushButtonArray[index].pinMask =  pinNumber;
			bushButtonArray[index].pushTimeStamp = 0;
			bushButtonArray[index].longPushStartThreshold = longPushStartThreshold*10;
			bushButtonArray[index].mediumPushThreshold = mediumPushThreshold*10;
			bushButtonArray[index].shortPushThreshold = shortPushThreshold*10;
			bushButtonArray[index].pushButtonCallback = pushButtonCallback;

			return true;
		}
	}
	return false;
}

uint32_t  enable_push_button(uint32_t pinNumber, uint32_t enable)
{
	uint32_t pinMask = 0 << pinNumber;
	if (registeredButtonsMask & pinMask)
	{
		if (enable)
		{
			enabledButtonsMask |= pinMask;
		}
		else
		{
			uint32_t index;
			for (index = 0; index < NUM_OF_BUTTONS; index++)
			{
				if (bushButtonArray[index].pinNumber == pinNumber)
				{
					if (enabledButtonsMask & pressedButtonsMask & longPushIssuedMask & pinMask)
					{
						bushButtonArray[index].lastEvent = PUSH_HOLD_STOP;
						//button_timer_arm(&bushButtonArray[index].pushButtonTimer, &bushButtonArray[index].pushButtonCallback, &bushButtonArray[index].lastEvent);
						bushButtonArray[index].pushButtonCallback(PUSH_HOLD_STOP);
						longPushIssuedMask &= ~pinMask;
					}
					bushButtonArray[index].pushTimeStamp = 0;
					break;
				}
			}
			
			enabledButtonsMask &= ~pinMask;
			pressedButtonsMask &= ~pinMask;
		}
		return true;
	}
	return false;
}
	


void  push_button_handler_cb(pushButtonConfig_t * pushButton)
{
	//pushButtonConfig_t * pushButton;
	//pushButton = (pushButtonConfig_t *) parg;
	if (NULL != pushButton->pushButtonCallback)
	{
		if (PUSH_START == pushButton->lastEvent)
		{
			pushButton->lastEvent = PUSH_HOLD_START;
			longPushIssuedMask |= pushButton->pinMask;
			pushButton->pushButtonCallback(pushButton->lastEvent);
		}
		else if ((PUSH_HOLD_STOP == pushButton->lastEvent) && (longPushIssuedMask |= pushButton->pinMask))
		{
			longPushIssuedMask &= ~pushButton->pinMask;
			pushButton->pushButtonCallback(pushButton->lastEvent);
		}
		else
		{
			pushButton->pushButtonCallback(pushButton->lastEvent);
		}
	}
}


void update_button_states(void)
{
	static uint32_t previousInputStatus = 0xFFFFFFFF;
	static uint32_t previousActiveButtonsCount = 0;
	uint32_t currentActiveButtonsCount = 0;
	uint32_t gpioInputStatus = GPIOA->IDR;
	uint32_t pressedButtons = (~gpioInputStatus) & previousInputStatus;
	uint32_t releasedButtons = gpioInputStatus & (~previousInputStatus);
	pushButtonConfig_t * pushButton;
	
	uint32_t systemTimeStamp = HAL_GetTick(); 
	
	uint32_t index;
	for (index = 0; index < NUM_OF_BUTTONS; index++)
	{
		pushButton = &bushButtonArray[index];
		
		if (pushButton->pushButtonCallback != NULL)
		{	
			if (pressedButtons & pushButton->pinMask)
			{
				if (displayActivateScreen())
				{
					
				}
				pushButton->pushTimeStamp = systemTimeStamp;
				pushButton->lastEvent = PUSH_START;
				
				if (pushButton->longPushStartThreshold != 0)
				{

				}
			}
			else if (releasedButtons & pushButton->pinMask)
			{
				uint32_t timeSinceLastPush = 0;

				pushButton->releaseTimeStamp = systemTimeStamp;
				
				if (pushButton->pushTimeStamp < systemTimeStamp)
				{
					timeSinceLastPush = systemTimeStamp - pushButton->pushTimeStamp;
				}
				else
				{
					timeSinceLastPush = (0xFFFFFFFF - pushButton->pushTimeStamp) + systemTimeStamp;
				}
				
				if ((pushButton->longPushStartThreshold != 0) && (timeSinceLastPush > pushButton->longPushStartThreshold))
				{

							pushButton->lastEvent = PUSH_HOLD_STOP;
							push_button_handler_cb(pushButton);

				}
				else if ((pushButton->mediumPushThreshold != 0) && (timeSinceLastPush >= pushButton->mediumPushThreshold))
				{
					//pushButton->lastEvent = PUSH_MEDIUM;
					//push_button_handler_cb(pushButton);
				}
				else if ((pushButton->shortPushThreshold != 0) && (timeSinceLastPush >= pushButton->shortPushThreshold))
				{
					pushButton->lastEvent = PUSH_SHORT;
					push_button_handler_cb(pushButton);
				}
			}
			
			if ((~gpioInputStatus) & pushButton->pinMask)
			{
				currentActiveButtonsCount++;
				pushButton->currentState = 1;

				uint32_t timeSinceLastPush = 0;

				pushButton->releaseTimeStamp = systemTimeStamp;
				
				if (pushButton->pushTimeStamp < systemTimeStamp)
				{
					timeSinceLastPush = systemTimeStamp - pushButton->pushTimeStamp;
				}
				else
				{
					timeSinceLastPush = (0xFFFFFFFF - pushButton->pushTimeStamp) + systemTimeStamp+1;
				}
				
				if ((pushButton->longPushStartThreshold != 0) && (timeSinceLastPush > pushButton->longPushStartThreshold))
				{
					

					if ((HAL_GPIO_ReadPin(GPIOA, SW5_Pin) == 0) && (HAL_GPIO_ReadPin(GPIOA, SW4_Pin) == 0))
					{ /*Enter OFFICE*/

						releasedButtons = 0; 
						pressedButtons = 0;
						gpioInputStatus = 0xFFEA;
						
					}
					else if ((HAL_GPIO_ReadPin(GPIOA, SW5_Pin) == 0) && (HAL_GPIO_ReadPin(GPIOA, SW3_Pin) == 0))
					{ /*Enter HOME*/

						releasedButtons = 0; 
						pressedButtons = 0;
						gpioInputStatus = 0xFFEA;
						
					}
					else if ((HAL_GPIO_ReadPin(GPIOA, SW4_Pin) == 0) && (HAL_GPIO_ReadPin(GPIOA, SW3_Pin) == 0))
					{ /*Enter LOCK*/
						
						displayTripleBlinker(16);//the blinker182
						if(display_status.currentState == LOCK)
							display_status.currentState = COMFORT;
						else
							display_status.currentState = LOCK;//this enters lock mode

						releasedButtons = 0; 
						pressedButtons = 0;
						gpioInputStatus = 0xFFEA;
					}
					
					else {
						
						releasedButtons = 0; 
						pressedButtons = 0;
						gpioInputStatus = 0xFFEA;
						
					pushButton->lastEvent = PUSH_HOLD_STOP;
					push_button_handler_cb(pushButton);
						while(BUTTON_5_IS_PRESSED)
							display();
					}
				}
				else if ((pushButton->mediumPushThreshold != 0) && (timeSinceLastPush >= pushButton->mediumPushThreshold))
				{
					if ((HAL_GPIO_ReadPin(GPIOA, SW5_Pin) == 0) && (HAL_GPIO_ReadPin(GPIOA, SW2_Pin) == 0))
					{						
						releasedButtons = 0; 
						pressedButtons = 0;
						gpioInputStatus = 0xFFEA;					
					}
					else{
					}
				}						
			}
			else	
			{
				pushButton->currentState = 0;
			}
		}
	}

	if (previousActiveButtonsCount != currentActiveButtonsCount)
	{
		if (multi_button_cb != NULL)
		{
			if ((previousActiveButtonsCount > 0) && (currentActiveButtonsCount > 1))
			{
				multi_button_cb_checker();
			}
		}
		previousActiveButtonsCount = currentActiveButtonsCount;
	}

	previousInputStatus = gpioInputStatus;
	
}
