/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 * @defgroup gpio_example_pca10001_main main.c
 * @{
 * @ingroup gpio_example_pca10001
 *
 * @brief GPIO Example Application main file.
 *
 * This file contains the source code for a sample application using GPIO to drive LEDs.
 *
 */
#include "nrf_delay.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

#include "nrf_gpio.h"
//#include <boards/pca10001.h>
#include "LEDBoard.h"
#include "light_ws2812_cortex.h"
#include "LEDVM.h"
#include "alloca.h"

LEDProgram ledProgram;

void led_init() {
	nrf_gpio_cfg_output(LED1_PIN);
    memset((void *) &ledProgram,0,sizeof(ledProgram));

    ledProgram.pc = 0;
    ledProgram.strips[0].count = 15;
    ledProgram.strips[0].brightness = 64;
    ledProgram.state =  STATE_IDLE;
    ledProgram.mode  = MODE_OFF;
}

void setIdle(LEDProgram* program) {
  if (program->state == STATE_RUNNING) {
	// turn off all led's when going from running to idle
	memset(&program->strips[0].pixels, 0, sizeof(program->strips[0].pixels));
	program->strips[0].sync = 1;// enable a 1 time sync
	program->state = STATE_IDLE;
  } else {
    error("Can't enter idle state from state %d\n",program->state);
  }
}

void setRunning(LEDProgram *program) {
  if (program->state == STATE_IDLE) {
	  // reset the PC when transitioning to running
	  program->pc = 0;
	program->state = STATE_RUNNING;
  } else {
    error("ERROR can't run program from state %d\n",program->state);
  }
}


/**
 *@}
 **/
