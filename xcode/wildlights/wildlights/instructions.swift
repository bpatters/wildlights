//
//  static let INSTructions.swift
//  wildlights
//
//  Created by Bret Patterson on 5/24/15.
//  Copyright (c) 2015 Bret Patterson. All rights reserved.
//
class Instructions {
    static let INST_END          = 0        // END Program
    static let INST_DELAY        = 1        // wait [short] milliseconds
    static let INST_PUSH         = 2        // PUSH reg onto stack
    static let INST_PUSHN        = 3        // PUSHN r[n1],r[n2]   - push r[n1] -->r[n2] onto the stack ie pushn r0,r12
    static let INST_POP          = 4        // POP reg from stack
    static let INST_POPN         = 5        // POPN r[n1],r[n2]   - push r[n1] -->r[n2] from the stack ie popn r0,r12
    static let INST_MOVIB        = 6        // MOV dreg,imm8  - move immediate value into register
    static let INST_MOVIS        = 7        // MOV dreg,imm16 - move immediate value into register
    static let INST_MOVI         = 8        // MOV dreg,imm32 - move immediate value into register
    static let INST_MOV          = 9        // MOV dreg,sreg  - move register to register - 32 bit
    static let INST_SUBIB        = 10        // SUB reg,imm8   - reg = reg - imm
    static let INST_SUBIS        = 11        // SUB reg,imm16  - reg = reg - imm
    static let INST_SUBI         = 12        // SUB reg,imm32  - reg = reg - imm
    static let INST_SUB          = 13        // SUB dreg,sreg  - dreg = dreg - sreg
    static let INST_ADDIB        = 14        // ADD reg,imm8   - reg = reg + imm
    static let INST_ADDIS        = 15        // ADD reg,imm16  - reg = reg + imm
    static let INST_ADDI         = 16        // ADD reg,imm32  - reg = reg + imm
    static let INST_ADD          = 17        // ADD dreg,sreg  - dreg = dreg + sreg
    static let INST_MULIB        = 18        // MUL reg,imm8   - reg = reg * imm
    static let INST_MULIS        = 19        // MUL reg,imm16  - reg = reg * imm
    static let INST_MULI         = 20        // MUL reg,imm32  - reg = reg * imm
    static let INST_MUL          = 21        // MUL dreg,sreg  - dreg = dreg * sreg
    static let INST_DIVIB        = 22        // MUL reg,imm8   - reg = reg / imm
    static let INST_DIVIS        = 23        // MUL reg,imm16  - reg = reg / imm
    static let INST_DIVI         = 24        // MUL reg,imm32  - reg = reg / imm
    static let INST_DIV          = 25        // MUL dreg,sreg  - dreg = dreg / sreg
    static let INST_SHIFTL       = 26        // SHIFTL dreg,imm8   dreg = dreg << imm8
    static let INST_SHIFTR       = 27        // SHIFTL dreg,imm8   dreg = dreg << imm8
    static let INST_ANDIB        = 28        // ANDI reg,imm8     BITWISE AND
    static let INST_ANDIS        = 29        // ANDI reg,imm16     BITWISE AND
    static let INST_ANDI         = 30        // ANDI reg,imm32     BITWISE AND
    static let INST_AND          = 31        // AND  dreg,sreg  BITWISE AND dreg = dreg & sreg;
    static let INST_ORIB         = 32        // OR reg,imm8 BITWISE OR
    static let INST_ORIS         = 33        // OR reg,imm16 BITWISE OR
    static let INST_ORI          = 34        // OR reg,imm32 BITWISE OR
    static let INST_OR           = 35        // OR dreg,sreg dreg = dreg | sreg
    static let INST_XORIB        = 36        // XOR reg,imm8 BITWISE XOR
    static let INST_XORIS        = 37        // XOR reg,imm16 BITWISE XOR
    static let INST_XORI         = 38        // XOR reg,imm32 BITWISE XOR
    static let INST_XOR          = 39        // XOR dreg,sreg BITWISE XOR dreg = dreg ^ sreg
    static let INST_NOT          = 40        // NOT dreg   dreg = ~dreg
    
    static let INST_JMP          = 41        // JMP [2 uint8_t offset]
    static let INST_CALL         = 42        // CALL imm  implicit PUSHS [pc]        = 0,JMP imm
    static let INST_RET          = 43        // return to address on top of stack implicit pop imm --> pc
    static let INST_CMPIB        = 44        // cmp reg,imm8  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg [=,>,<] imm8
    static let INST_CMPIS        = 45        // cmp reg,imm16 = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg [=,>,<] imm8
    static let INST_CMPI         = 46        // cmp reg,imm32  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg [=,>,<] imm8
    static let INST_CMP          = 47        // cmp reg1,reg2  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg1 [=,>,<] reg2
    static let INST_JEQ          = 48        // Jump addr - jump to address if FLAG = FLAG_EQ
    static let INST_JNE          = 49        // Jump addr - jump to address if FLAG != FLAG_EQ
    static let INST_JLT          = 50        // Jump addr - jump to address if FLAG = FLAG_LT
    static let INST_JGT          = 51        // Jump addr - jump to address if FLAG = FLAG_0
    
    // LED manipulation functions
    static let INST_PUSHCR       = 52        // imm8,reg - push (reg = xrgb) pixels out of head unit into led#
    static let INST_PULLCR       = 53        // imm8,reg - pull a pixel into head unit and shift in from end of strip (reg = xrgb)
    static let INST_ROTATEO      = 54        // imm8     - ledStrip# rotate 1 leds away from head unit and those colors shifted out come back in
    static let INST_ROTATEI      = 55        // imm8     - ledStrip# rotate 1 leds into head unit and those colors shifted in goto end of strip
    static let INST_SETPIXEL     = 56        // imm8,reg1,reg2 - For LED strip imm8 SET LED=reg1 to to xrgb color = reg2
    static let INST_READPIXEL    = 57        // imm8,dreg,src - For LED strip imm8 READ LED=sreg into dreg
    static let INST_UPDATE       = 58        // imm8   -- Sends pixel updates to the specified strip number
}