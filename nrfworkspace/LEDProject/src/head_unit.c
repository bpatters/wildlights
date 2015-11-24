//
// Created by Bret Patterson on 10/11/15.
//

#include "head_unit.h"
#include "LEDVM.h"
#include <string.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"


head_unit_t head_unit;


void head_unit_init() {

    head_unit.transition_to_mode = MODE_IDLE;
    head_unit.current_mode = MODE_OFF;

    nrf_gpio_cfg_output(LED1_PIN);
    memset((void *) &head_unit.program, 0, sizeof(head_unit.program));
    head_unit.program.pc = 0;
    head_unit.program.strips[0].count = 15;
    head_unit.program.strips[0].brightness = 64;
    head_unit.program.strips[0].sync = false;
    head_unit.program.state = STATE_RUNNING;
}

void head_unit_set_program_running(head_unit_t *head_unit) {
    head_unit->program.state = STATE_RUNNING;
}

void head_unit_set_program_idle(head_unit_t *head_unit) {
    head_unit->program.state = STATE_IDLE;
}

uint8_t head_unit_process_transition(head_unit_t *head_unit) {
    // first handle the power off process before doing anything else
    switch (head_unit->current_mode) {
        case MODE_POWER_ON_0:
            // set to white
            led_vm_set_all(&head_unit->program, 0, 255, 255, 255);
            led_vm_sync(&head_unit->program, 0);
            return head_unit->current_mode = MODE_POWER_ON_1;
        case MODE_POWER_ON_1:
            delay(POWER_ON_FLASH_DELAY);
            // set to black
            led_vm_set_all(&head_unit->program, 0, 0, 0, 0);
            led_vm_sync(&head_unit->program, 0);
            return head_unit->current_mode = MODE_POWER_ON_2;
        case MODE_POWER_ON_2:
            delay(POWER_ON_FLASH_DELAY);
            // set to white
            led_vm_set_all(&head_unit->program, 0, 255, 255, 255);
            led_vm_sync(&head_unit->program, 0);
            return head_unit->current_mode = MODE_POWER_ON_3;
        case MODE_POWER_ON_3:
            delay(POWER_ON_FLASH_DELAY);
            // set to black
            led_vm_set_all(&head_unit->program, 0, 0, 0, 0);
            led_vm_sync(&head_unit->program, 0);
            return head_unit->current_mode = MODE_POWER_ON_4;
        case MODE_POWER_ON_4:
            delay(POWER_ON_FLASH_DELAY);
            // set to white
            led_vm_set_all(&head_unit->program, 0, 255, 255, 255);
            led_vm_sync(&head_unit->program, 0);
            return head_unit->current_mode = MODE_POWER_ON_5;
        case MODE_POWER_ON_5:
            delay(POWER_ON_FLASH_DELAY);
            // set to white
            led_vm_set_all(&head_unit->program, 0, 0, 0, 0);
            led_vm_sync(&head_unit->program, 0);
            return head_unit->current_mode = MODE_IDLE;
        case MODE_POWER_OFF_0:
            // set to green
            led_vm_set_all(&head_unit->program, 0, 255, 0, 0);
            led_vm_sync(&head_unit->program, 0);
            return head_unit->current_mode = MODE_POWER_OFF_1;
        case MODE_POWER_OFF_1:
            delay(POWER_OFF_FLASH_DELAY);
            // set to white
            led_vm_set_all(&head_unit->program, 0, 0, 0, 0);
            led_vm_sync(&head_unit->program, 0);
            return head_unit->current_mode = MODE_OFF;
        default:
            break;
    }

    if (head_unit->transition_to_mode == MODE_OFF) {
        return head_unit->current_mode = MODE_POWER_OFF_0;
    }
    if ((head_unit->current_mode == MODE_OFF) && (head_unit->transition_to_mode == MODE_IDLE)) {
        return head_unit->current_mode = MODE_POWER_ON_0;
    }

    if (head_unit->current_mode == MODE_RUN && head_unit->transition_to_mode == MODE_IDLE) {
        led_vm_set_all(&head_unit->program, 0, 0, 0, 0);
        led_vm_sync(&head_unit->program, 0);
    }


    // no need for special logic since only allowed transitions left
    // we do allow invalid transitions to middle of power off cycle, but once in the power off cycle there is no aborting until done
    head_unit->current_mode = head_unit->transition_to_mode;

    return head_unit->current_mode;
}