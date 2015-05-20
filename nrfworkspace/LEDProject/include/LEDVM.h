#ifndef LEDVM_H
#define LEDVM_H

#include "nrf_delay.h"
#include <stdint.h>
#include <stdbool.h>
#include "platform.h"
#include "global.h"
#include "led_program.h"
#include "led_strip.h"


// bitwise flag for different operations
#define FLAG_EQ 0
#define FLAG_GT 1
#define FLAG_LT 2

typedef enum  {
	INST_END,        // END Program
	INST_DELAY,      // wait [short] milliseconds
	INST_PUSH,       // PUSH reg onto stack
	INST_PUSHN, // PUSHN r[n1],r[n2]   - push r[n1] -->r[n2] onto the stack ie pushn r0,r12
	INST_POP,        // POP reg from stack
	INST_POPN, // POPN r[n1],r[n2]   - push r[n1] -->r[n2] from the stack ie popn r0,r12
	INST_MOVIB,      // MOV dreg,imm8  - move immediate value into register
	INST_MOVIS,      // MOV dreg,imm16 - move immediate value into register
	INST_MOVI,       // MOV dreg,imm32 - move immediate value into register
	INST_MOV,        // MOV dreg,sreg  - move register to register - 32 bit
	INST_SUBIB,      // SUB reg,imm8   - reg = reg - imm
	INST_SUBIS,      // SUB reg,imm16  - reg = reg - imm
	INST_SUBI,       // SUB reg,imm32  - reg = reg - imm
	INST_SUB,        // SUB dreg,sreg  - dreg = dreg - sreg
	INST_ADDIB,      // ADD reg,imm8   - reg = reg + imm
	INST_ADDIS,      // ADD reg,imm16  - reg = reg + imm
	INST_ADDI,       // ADD reg,imm32  - reg = reg + imm
	INST_ADD,        // ADD dreg,sreg  - dreg = dreg + sreg
	INST_MULIB,      // MUL reg,imm8   - reg = reg * imm
	INST_MULIS,      // MUL reg,imm16  - reg = reg * imm
	INST_MULI,       // MUL reg,imm32  - reg = reg * imm
	INST_MUL,        // MUL dreg,sreg  - dreg = dreg * sreg
	INST_DIVIB,      // MUL reg,imm8   - reg = reg / imm
	INST_DIVIS,      // MUL reg,imm16  - reg = reg / imm
	INST_DIVI,       // MUL reg,imm32  - reg = reg / imm
	INST_DIV,        // MUL dreg,sreg  - dreg = dreg / sreg
	INST_SHIFTL,     // SHIFTL dreg,imm8   dreg = dreg << imm8
	INST_SHIFTR,     // SHIFTL dreg,imm8   dreg = dreg << imm8
	INST_ANDIB,      // ANDI reg,imm8     BITWISE AND
	INST_ANDIS,      // ANDI reg,imm16     BITWISE AND
	INST_ANDI,       // ANDI reg,imm32     BITWISE AND
	INST_AND,        // AND  dreg,sreg  BITWISE AND dreg = dreg & sreg;
	INST_ORIB,       // OR reg,imm8 BITWISE OR  
	INST_ORIS,       // OR reg,imm16 BITWISE OR
	INST_ORI,        // OR reg,imm32 BITWISE OR
	INST_OR,         // OR dreg,sreg dreg = dreg | sreg
	INST_XORIB,      // XOR reg,imm8 BITWISE XOR
	INST_XORIS,      // XOR reg,imm16 BITWISE XOR
	INST_XORI,       // XOR reg,imm32 BITWISE XOR
	INST_XOR,        // XOR dreg,sreg BITWISE XOR dreg = dreg ^ sreg
	INST_NOT,        // NOT dreg   dreg = ~dreg

	INST_JMP,        // JMP [2 uint8_t offset]
	INST_CALL,       // CALL imm  implicit PUSHS [pc], JMP imm
	INST_RET,       // return to address on top of stack implicit pop imm --> pc
	INST_CMPIB, // cmp reg,imm8  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg [=,>,<] imm8
	INST_CMPIS, // cmp reg,imm16 = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg [=,>,<] imm8
	INST_CMPI, // cmp reg,imm32  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg [=,>,<] imm8
	INST_CMP, // cmp reg1,reg2  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg1 [=,>,<] reg2
	INST_JEQ,        // Jump addr - jump to address if FLAG = FLAG_EQ
	INST_JNE,        // Jump addr - jump to address if FLAG != FLAG_EQ
	INST_JLT,        // Jump addr - jump to address if FLAG = FLAG_LT
	INST_JGT,        // Jump addr - jump to address if FLAG = FLAG_0

// LED manipulation functions
	INST_PUSHCR, // imm8,reg - push (reg = xrgb) pixels out of head unit into led#
	INST_PULLCR, // imm8,reg - pull a pixel into head unit and shift in from end of strip (reg = xrgb)
	INST_ROTATEO, // imm8     - ledStrip# rotate 1 leds away from head unit and those colors shifted out come back in
	INST_ROTATEI, // imm8     - ledStrip# rotate 1 leds into head unit and those colors shifted in goto end of strip
	INST_SETPIXEL, // imm8,reg1,reg2 - For LED strip imm8 SET LED=reg1 to to xrgb color = reg2
	INST_READPIXEL, // imm8,dreg,src - For LED strip imm8 READ LED=sreg into dreg
	INST_UPDATE   // imm8   -- Sends pixel updates to the specified strip number
}INSTRUCTIONS;

/********* PROGRAM INSTRUCTIONS **********/
void exec_INST_ROTATEO(LEDProgram* program, uint8_t stripNum);
void exec_INST_ROTATEI(LEDProgram* program, uint8_t stripNum);
void setPixelColor(LEDStrip* strip, uint16_t pixel, uint8_t r, uint8_t g, uint8_t b);
uint32_t getPixelColor(LEDStrip* strip, uint16_t pixel);
int8_t readIMM8(LEDProgram* program);
int16_t readIMM16(LEDProgram* program);
int32_t readIMM32(LEDProgram* program);
float readFloat(LEDProgram* program);
void runProgram(LEDProgram* program);

#endif
