/*
 * instructions.h
 *
 *  Created on: May 24, 2015
 *      Author: bpatterson
 */

#ifndef INCLUDE_INSTRUCTIONS_H_
#define INCLUDE_INSTRUCTIONS_H_


typedef enum  {
	INST_END		 = 0,        // END Program
	INST_DELAY		 = 1,     // wait [short] milliseconds
	INST_PUSH		 = 2,      // PUSH reg onto stack
	INST_PUSHN		 = 3, // PUSHN r[n1],r[n2]   - push r[n1] -->r[n2] onto the stack ie pushn r0,r12
	INST_POP		 = 4,       // POP reg from stack
	INST_POPN		 = 5, // POPN r[n1],r[n2]   - push r[n1] -->r[n2] from the stack ie popn r0,r12
	INST_MOVIB		 = 6,      // MOV dreg,imm8  - move immediate value into register
	INST_MOVIS		 = 7,     // MOV dreg,imm16 - move immediate value into register
	INST_MOVI		 = 8,      // MOV dreg,imm32 - move immediate value into register
	INST_MOV		 = 9,       // MOV dreg,sreg  - move register to register - 32 bit
	INST_SUBIB		 = 10,     // SUB reg,imm8   - reg = reg - imm
	INST_SUBIS		 = 11,     // SUB reg,imm16  - reg = reg - imm
	INST_SUBI		 = 12,      // SUB reg,imm32  - reg = reg - imm
	INST_SUB		 = 13,       // SUB dreg,sreg  - dreg = dreg - sreg
	INST_ADDIB		 = 14,     // ADD reg,imm8   - reg = reg + imm
	INST_ADDIS		 = 15,     // ADD reg,imm16  - reg = reg + imm
	INST_ADDI		 = 16,      // ADD reg,imm32  - reg = reg + imm
	INST_ADD		 = 17,       // ADD dreg,sreg  - dreg = dreg + sreg
	INST_MULIB		 = 18,     // MUL reg,imm8   - reg = reg * imm
	INST_MULIS		 = 19,     // MUL reg,imm16  - reg = reg * imm
	INST_MULI		 = 20,      // MUL reg,imm32  - reg = reg * imm
	INST_MUL		 = 21,       // MUL dreg,sreg  - dreg = dreg * sreg
	INST_DIVIB		 = 22,     // MUL reg,imm8   - reg = reg / imm
	INST_DIVIS		 = 23,     // MUL reg,imm16  - reg = reg / imm
	INST_DIVI		 = 24,      // MUL reg,imm32  - reg = reg / imm
	INST_DIV		 = 25,       // MUL dreg,sreg  - dreg = dreg / sreg
	INST_SHIFTL		 = 26,    // SHIFTL dreg,imm8   dreg = dreg << imm8
	INST_SHIFTR		 = 27,    // SHIFTL dreg,imm8   dreg = dreg << imm8
	INST_ANDIB		 = 28,     // ANDI reg,imm8     BITWISE AND
	INST_ANDIS		 = 29,     // ANDI reg,imm16     BITWISE AND
	INST_ANDI		 = 30,      // ANDI reg,imm32     BITWISE AND
	INST_AND		 = 31,       // AND  dreg,sreg  BITWISE AND dreg = dreg & sreg;
	INST_ORIB		 = 32,      // OR reg,imm8 BITWISE OR
	INST_ORIS		 = 33,      // OR reg,imm16 BITWISE OR
	INST_ORI		 = 34,       // OR reg,imm32 BITWISE OR
	INST_OR		 	 = 35,        // OR dreg,sreg dreg = dreg | sreg
	INST_XORIB		 = 36,     // XOR reg,imm8 BITWISE XOR
	INST_XORIS		 = 37,     // XOR reg,imm16 BITWISE XOR
	INST_XORI		 = 38,      // XOR reg,imm32 BITWISE XOR
	INST_XOR		 = 39,       // XOR dreg,sreg BITWISE XOR dreg = dreg ^ sreg
	INST_NOT		 = 40,       // NOT dreg   dreg = ~dreg

	INST_JMP		 = 41,       // JMP [2 uint8_t offset]
	INST_CALL		 = 42,      // CALL imm  implicit PUSHS [pc]		 = JMP imm
	INST_RET		 = 43,      // return to address on top of stack implicit pop imm --> pc
	INST_CMPIB		 = 44, // cmp reg,imm8  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg [=,>,<] imm8
	INST_CMPIS		 = 45, // cmp reg,imm16 = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg [=,>,<] imm8
	INST_CMPI		 = 46, // cmp reg,imm32  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg [=,>,<] imm8
	INST_CMP		 = 47, // cmp reg1,reg2  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg1 [=,>,<] reg2
	INST_JEQ		 = 48,       // Jump addr - jump to address if FLAG = FLAG_EQ
	INST_JNE		 = 49,       // Jump addr - jump to address if FLAG != FLAG_EQ
	INST_JLT		 = 50,       // Jump addr - jump to address if FLAG = FLAG_LT
	INST_JGT		 = 51,       // Jump addr - jump to address if FLAG = FLAG_0

// LED manipulation functions
	INST_PUSHCR		 = 52, // imm8,reg - push (reg = xrgb) pixels out of head unit into led#
	INST_PULLCR		 = 53, // imm8,reg - pull a pixel into head unit and shift in from end of strip (reg = xrgb)
	INST_ROTATEO	 = 54, // imm8     - ledStrip# rotate 1 leds away from head unit and those colors shifted out come back in
	INST_ROTATEI	 = 55, // imm8     - ledStrip# rotate 1 leds into head unit and those colors shifted in goto end of strip
	INST_SETPIXEL	 = 56, //  imm8,reg1,reg2 - For LED strip imm8 SET LED=reg1 to to xrgb color = reg2
	INST_READPIXEL	 = 57, // imm8,dreg,src - For LED strip imm8 READ LED=sreg into dreg
	INST_UPDATE 	 = 58  // imm8   -- Sends pixel updates to the specified strip number
} INSTRUCTIONS;
#endif /* INCLUDE_INSTRUCTIONS_H_ */
