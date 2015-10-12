#ifndef LED_PROGRAM_H
#define LED_PROGRAM_H
#include "led_strip.h"

#define STATE_IDLE    	      0 // do nothing
#define STATE_RUNNING 	      1 // running
#define STATE_PROGRAMMING     2 // being programmed

#define RC_OK              0
#define RC_STACK_UNDERFLOW 1
#define RC_STACK_OVERFLOW  2

// sizeOf LEDProgram = 1993
typedef struct _LEDProgram {
	uint8_t state;        	// 1    uint8_t state
	uint16_t pc;          	// 2    uint8_t pc
	uint8_t program[1024];  // 1024 of program space
	uint32_t sp;            // 4
	uint32_t stack[16];     // 64
	int32_t vars[32];    	// 128  uint8_t of 32 bit variables
	uint32_t flags;    		// 4    uint8_t of flags
	LEDStrip strips[1];   	// 768 = 3+ 3*255 num leds*3
} LEDProgram;

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

#endif
