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

    static uint8_t testProgram[] =
    {
       8, 31, 0x00,0x00,0x00,0xFF,           //         movi r31,0x000000FF
       8, 30, 0x00,0x00,0x00,0x01,           //         movi r30,0x00000001
       8, 31, 0x00,0x00,0x00,0xFF,           //         movi r31,0x000000FF
      52, 0x00, 31,                          //       pushcr 0x00,r31
      58, 0x00,                              //       update 0x00
       1, 0x00,0x21,                         //        delay 0x0021
      26, 31, 0x08,                          //       shiftl r31,0x08
      52, 0x00, 31,                          //       pushcr 0x00,r31
      58, 0x00,                              //       update 0x00
       1, 0x00,0x21,                         //        delay 0x0021
      26, 31, 0x08,                          //       shiftl r31,0x08
      52, 0x00, 31,                          //       pushcr 0x00,r31
      58, 0x00,                              //       update 0x00
       1, 0x00,0x21,                         //        delay 0x0021
      41, 0x00,0x0C                          //          jmp 0x000C
    };

    memcpy(ledProgram.program, testProgram, sizeof(testProgram));
    ledProgram.pc = 0;

    ledProgram.strips[0].count = 15;
    ledProgram.strips[0].brightness = 64;

    info("initialized strip[0] count[%d] pixels[%p] brightness[%d]\n",ledProgram.strips[0].count,ledProgram.strips[0].pixels,ledProgram.strips[0].brightness);
    ledProgram.state =  STATE_IDLE;
    ledProgram.mode = MODE_RUN_WHEN_DISCONNECT;
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
