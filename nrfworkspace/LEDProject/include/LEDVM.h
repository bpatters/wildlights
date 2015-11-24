#ifndef LEDVM_H
#define LEDVM_H

#include <stdint.h>
#include <stdbool.h>
#include "platform.h"
#include "global.h"
#include "led_program.h"
#include "led_strip.h"
#include "instructions.h"
// bitwise flag for different operations
#define FLAG_EQ 0
#define FLAG_GT 1
#define FLAG_LT 2


/********* PROGRAM INSTRUCTIONS **********/
void runProgram(LEDProgram* program);
void runSingleInstruction(LEDProgram* program, uint16_t* pc, uint8_t* byteCodes);


void setPixelColor(LEDStrip* strip, uint16_t pixel, uint8_t r, uint8_t g, uint8_t b);
uint32_t getPixelColor(LEDStrip* strip, uint16_t pixel);
int8_t readIMM8(uint16_t *pc, uint8_t* byteCodes);
int16_t readIMM16(uint16_t *pc, uint8_t* byteCodes);
int32_t readIMM32(uint16_t *pc, uint8_t* byteCodes);
float readFloat(uint16_t *pc, uint8_t* byteCodes);

void exec_INST_ROTATEO(LEDProgram* program, uint8_t stripNum);
void exec_INST_ROTATEI(LEDProgram* program, uint8_t stripNum);

/** Animation step functions **/
void push_color(LEDProgram* program, uint8_t stripNum, uint8_t red, uint8_t green, uint8_t blue);
void pull_color(LEDProgram* program, uint8_t stripNum, uint8_t red, uint8_t green, uint8_t blue);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Set all LED's to specified color
 */
void led_vm_set_all(LEDProgram* program, uint8_t stripNum, uint8_t red, uint8_t green, uint8_t blue);

/**
 * Sync all colors to the LED Strip. IE
 */
void led_vm_sync(LEDProgram* program, uint8_t stripNum);


#ifdef __cplusplus
}
#endif
#endif
