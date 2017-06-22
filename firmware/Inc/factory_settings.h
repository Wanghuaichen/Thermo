#ifndef __factory_settings_H
#define __factory_settings_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern bool factorySettingsActive;
extern bool factorySettingsDownTheDrainHole;
extern uint16_t factorySettingsRound[];
extern uint16_t factorySettingsSetRound;



void factorySettingsDisplayTimer(void);
void factorySettingsStateDisplay(void);


#ifdef __cplusplus
}
#endif
#endif


