//
// Created by Bret Patterson on 10/11/15.
//

#ifndef LEDPROJECT_HEAD_UNIT_H_H
#define LEDPROJECT_HEAD_UNIT_H_H

#include <stdbool.h>
#include <stdint.h>
#include "global.h"
#include "led_program.h"

#define POWER_OFF_FLASH_DELAY 1000
#define POWER_ON_FLASH_DELAY 250

/**
 * Transition states:
 * ANY --> MODE_POWER_OFF_1 --> MODE_POWER_OFF_2 --> MODE_POWER_OFF_3 --> MODE_POWER_OFF_4 --> MODE_POWER_OFF_5 --> MODE_OFF
 * MODE_IDLE --> *
 * MODE_RUN* --> *
 */
// defines triggers for state transitions
typedef enum {
    MODE_OFF,               // Turn off all led's and then go idle
    MODE_IDLE,              // not running a program, just waiting on commands
    MODE_RUN,               // running the current program
    MODE_RUN_ON_DISCONNECT, // waiting to enter disconnected state before running
    MODE_POWER_ON_0,
    MODE_POWER_ON_1,
    MODE_POWER_ON_2,
    MODE_POWER_ON_3,
    MODE_POWER_ON_4,
    MODE_POWER_ON_5,
    MODE_POWER_OFF_0,
    MODE_POWER_OFF_1
} head_unit_mode_t;

typedef struct _head_unit {
    bool connected;
    head_unit_mode_t transition_to_mode;
    head_unit_mode_t current_mode;
    LEDProgram program;
} head_unit_t;

extern head_unit_t head_unit;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the current mode
 */
void head_unit_init();

/**
 * set program state to idle
 */
void head_unit_set_program_idle();

/**
 * set program state to running
 */
void head_unit_set_program_running();

/**
 * Goes the process of powering off the head unit.
 * Flashes 3 times then sets current to MODE_OFF
 * @return the new current mode
 */
uint8_t head_unit_process_transition(head_unit_t *head_unit);

#ifdef __cplusplus
}
#endif

#endif //LEDPROJECT_HEAD_UNIT_H_H
