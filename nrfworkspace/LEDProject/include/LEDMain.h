#ifndef LEDMAIN_H
#define LEDMAIN_H
#include "LEDVM.h"
#include "led_program.h"

void led_init();
void setRunning(LEDProgram *program);
void setIdle(LEDProgram *program);
extern LEDProgram ledProgram;
#endif
