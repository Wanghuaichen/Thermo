#ifndef __BUTTON_DRV_H
#define __BUTTON_DRV_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
#define NUM_OF_BUTTONS 5 //Max possible 32

typedef enum { PUSH_START, PUSH_SHORT, PUSH_MEDIUM, PUSH_HOLD_START, PUSH_HOLD_STOP } push_event_t;
typedef void pushButtonCallback_t(push_event_t event);

typedef enum { MULTI_PUSH_ADD, MULTI_PUSH_REMOVE } multiPushEvent_t;

typedef struct
{
	unsigned int previousButtonMask;
	unsigned int currentButtonMask;
	unsigned int timeSincePreviousButtonMask;
	multiPushEvent_t multiPushEventType;
} multiPushButtonEventDetails_t;

typedef void multiPushButtonCallback_t(multiPushButtonEventDetails_t * eventDetails);

unsigned char get_last_mode(void);
void timer_arm_disarm(void);
void button_drv_init(void(* multiPushButtonCallback)(void));
unsigned int register_push_button(unsigned int pinNumber,
	unsigned int shortPushThreshold,
	unsigned int mediumPushThreshold, 
	unsigned int longPushStartThreshold,
	pushButtonCallback_t *pushButtonCallback);
unsigned int unregister_push_button(unsigned int pinNumber);
unsigned int enable_push_button(unsigned int pinNumber, unsigned int enable);
void update_button_states(void);
void register_button_press(unsigned int pinNumber);

#ifdef __cplusplus
}
 #endif

#endif //_BUTTON_DRV_H_
