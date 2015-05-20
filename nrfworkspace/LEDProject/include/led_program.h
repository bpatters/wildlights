#ifndef LED_PROGRAM_H
#define LED_PROGRAM_H
#include "led_strip.h"

#define STATE_IDLE    	      0 // do nothing
#define STATE_RUNNING 	      1 // running
#define STATE_PROGRAMMING     2 // being programmed

// defines triggers for state transitions
#define MODE_OFF                  0 // Turn off all led's and then go idle
#define MODE_IDLE                 1 // Go and stay idle at the next opportunity
#define MODE_RUN                  2 // Run at the next opportunity if not already running
#define MODE_RUN_WHEN_DISCONNECT  3 // waiting to enter disconnected state before running

typedef struct _LEDProgram {
	uint8_t mode;           // Mode the program operates in
	uint8_t state;        	// 1 uint8_t state
	uint16_t pc;          	// 2 uint8_t pc
	uint8_t program[1024];  // 2k of program space
	uint32_t sp;
	uint32_t stack[16];
	int32_t vars[32];    	// 1k uint8_t of 32 bit variables
	uint32_t flags;    		// 4 uint8_t of flags
	LEDStrip strips[1];   	// 4+ num leds*3
} LEDProgram;


#endif
