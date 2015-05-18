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
      42, 0x00,0x0F,                         //         call 0x000F
      42, 0x00,0x25,                         //         call 0x0025
      42, 0x00,0x3B,                         //         call 0x003B
      42, 0x00,0x51,                         //         call 0x0051
      41, 0x00,0x00,                         //          jmp 0x0000
       8, 0, 0x00,0x00,0x00,0xFF,            //         movi r0,0x000000FF
       8, 31, 0x00,0xFF,0xFF,0xFF,           //         movi r31,0x00FFFFFF
       8, 30, 0x00,0x00,0x00,0x01,           //         movi r30,0x00000001
      42, 0x00,0x67,                         //         call 0x0067
      43,                                    //          ret
       8, 0, 0x00,0x00,0x00,0xFF,            //         movi r0,0x000000FF
       8, 31, 0x00,0xFF,0x00,0x00,           //         movi r31,0x00FF0000
       8, 30, 0x00,0x00,0x00,0x01,           //         movi r30,0x00000001
      42, 0x00,0x67,                         //         call 0x0067
      43,                                    //          ret
       8, 0, 0x00,0x00,0x00,0xFF,            //         movi r0,0x000000FF
       8, 31, 0x00,0x00,0xFF,0x00,           //         movi r31,0x0000FF00
       8, 30, 0x00,0x00,0x00,0x01,           //         movi r30,0x00000001
      42, 0x00,0x67,                         //         call 0x0067
      43,                                    //          ret
       8, 0, 0x00,0x00,0x00,0xFF,            //         movi r0,0x000000FF
       8, 31, 0x00,0x00,0x00,0xFF,           //         movi r31,0x000000FF
       8, 30, 0x00,0x00,0x00,0x01,           //         movi r30,0x00000001
      42, 0x00,0x67,                         //         call 0x0067
      43,                                    //          ret
      42, 0x00,0x7D,                         //         call 0x007D
      13, 31, 30,                            //          sub r31,r30
      12, 0, 0x00,0x00,0x00,0x01,            //         subi r0,0x00000001
      44, 0, 0x00,                           //        cmpib r0,0x00
       1, 0x00,0x64,                         //        delay 0x0064
      49, 0x00,0x67,                         //          jne 0x0067
      43,                                    //          ret
       8, 1, 0x00,0x00,0x00,0x09,            //         movi r1,0x00000009
      52, 0x00, 31,                          //       pushcr 0x00,r31
      12, 1, 0x00,0x00,0x00,0x01,            //         subi r1,0x00000001
      44, 1, 0x00,                           //        cmpib r1,0x00
      49, 0x00,0x83,                         //          jne 0x0083
      58, 0x00,                              //       update 0x00
      43                                     //          ret
    };

    ledProgram.programSize = sizeof(testProgram);
    memcpy(ledProgram.program, testProgram, ledProgram.programSize);
    ledProgram.pc = 0;

    ledProgram.strips[0].count = 15;
    ledProgram.strips[0].brightness = 64;

    info("initialized strip[0] count[%d] pixels[%p] brightness[%d]\n",ledProgram.strips[0].count,ledProgram.strips[0].pixels,ledProgram.strips[0].brightness);
    ledProgram.state = STATE_RUNNING;
}





/** When we receive the Bluetooth begin programming packet we call this method to start programming **/
void beginProgramming(uint8_t led1Count,uint16_t programSize) {
  ledProgram.state = STATE_PROGRAMMING;
   // initialize for programming
  ledProgram.programSize = programSize;
  ledProgram.pc = 0;
  // initialize the LED1 system
  ledProgram.strips[0].count = led1Count;
}

/** When we receive the Bluetooth PROGRAM packet we call this function to append bytes to the program */
void appendProgram(uint8_t nsize,char *bytes) {
  if ((ledProgram.pc + nsize ) <= ledProgram.programSize) {
    memcpy(ledProgram.program+ledProgram.pc,bytes,nsize);
    ledProgram.pc += nsize;
  } else if (ledProgram.pc >= ledProgram.programSize) {
    error("Program too big, throwing away %d bytes.\n",nsize);
  } else {
    memcpy(ledProgram.program+ledProgram.pc,bytes,ledProgram.programSize-ledProgram.pc);
    ledProgram.pc = ledProgram.programSize;
    error("Program too big, throwing away %d bytes.\n",ledProgram.pc - ledProgram.programSize);
  }
}

/** When we receive the bluetooth END_PROGRAM packet we call this function */
void finishProgram() {
  ledProgram.state = STATE_IDLE;
}

void setIdle() {
  if (ledProgram.state == STATE_RUNNING) {
    ledProgram.state = STATE_IDLE;
  } else {
    error("Can't enter idle state from state %d\n",ledProgram.state);
  }
}

void setRunning() {
  if (ledProgram.state == STATE_IDLE) {
    ledProgram.state = STATE_RUNNING;
  } else {
    error("ERROR can't run program from state %d\n",ledProgram.state);
  }
}


/**
 *@}
 **/
