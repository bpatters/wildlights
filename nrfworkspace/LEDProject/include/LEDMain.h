#ifndef LEDMAIN_H
#define LEDMAIN_H
#include "LEDVM.h"
#include "led_program.h"

extern LEDProgram ledProgram;

void led_init();
void setRunning(LEDProgram *program);
void setIdle(LEDProgram *program);
#endif
