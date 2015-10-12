//
// Created by Bret Patterson on 10/11/15.
//

#ifndef LEDPROJECT_BUTTONS_H
#define LEDPROJECT_BUTTONS_H
#include "global.h"


#define BUTTON_PRESS   1
#define BUTTON_RELEASE 2

typedef void (*button_event_handler_t)(uint8_t button_pin);

#ifdef __cplusplus
extern "C" {
#endif

extern void buttons_init();
extern uint32_t buttons_enable(void);
extern uint32_t buttons_disable(void);

#ifdef __cplusplus
}
#endif

#endif //LEDPROJECT_BUTTONS_H
