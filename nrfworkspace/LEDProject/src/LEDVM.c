#include "LEDVM.h"


void runProgram(LEDProgram* program) {
	if (program->state != STATE_RUNNING)
		return;
	if (program->program == NULL)
		return;
	switch ((uint8_t) program->program[program->pc]) {
	case INST_END:
		program->state = STATE_IDLE;
		break;
	case INST_DELAY:
		program->pc++;
		{
			uint16_t d = program->program[program->pc++];
			d <<= 8;
			d += program->program[program->pc++];
			trace("executed INST_DELAY %d\n", d);
			delay(d);
		}
		break;
	case INST_PUSH:       // PUSH reg onto stack
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++];
			program->stack[program->sp++] = program->vars[reg];
			trace("push r%d = 0x%0.8x\n", reg, program->vars[reg]);
		}
		break;
	case INST_PUSHN:       // PUSH reg onto stack
		program->pc++;
		{
			uint8_t num1 = readIMM8(program);
			uint8_t num2 = readIMM8(program);
			for (uint8_t i = num1; i <= num2; i++) {
				program->stack[program->sp++] = program->vars[i];
				trace("push r%d = 0x%0.8x\n", i, program->vars[i]);
			}
		}
		break;
	case INST_POP:        // POP [long] from stack
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++];
			program->vars[reg] = program->stack[--program->sp];
			trace("POP r%d = 0x%0.8x\n", reg, program->vars[reg]);
		}
		break;
	case INST_POPN:        // POPN from stack
		program->pc++;
		{
			uint8_t num1 = readIMM8(program);
			uint8_t num2 = readIMM8(program);
			for (int i = num2; i >= num1; i--) {
				program->vars[i] = program->stack[--program->sp];
				trace("POP r%d = 0x%0.8x\n", i, program->vars[i]);
			}

		}
		break;
	case INST_MOVIB:      // MOV dreg,imm8  - move immediate value into register
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			program->vars[reg] = readIMM8(program);
			trace("MOVIB r%d =0x%0.8x\n", reg, program->vars[reg]);
		}
		break;
	case INST_MOVIS:      // MOV dreg,imm16 - move immediate value into register
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			program->vars[reg] = readIMM16(program);
			trace("MOVIS r%d = 0x%0.8x\n", reg, program->vars[reg]);
		}
		break;
	case INST_MOVI:
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			program->vars[reg] = readIMM32(program);

			trace("MOVI r%d = 0x%0.8x\n", reg, program->vars[reg]);
		}
		break;       // MOV dreg,imm32 - move immediate value into register
	case INST_MOV:        // MOV dreg,sreg  - move register to register - 32 bit
		program->pc++;
		{
			uint8_t dreg = program->program[program->pc++]; //reg number
			uint8_t sreg = program->program[program->pc++];
			program->vars[dreg] = program->vars[sreg];
			trace("MOVIB r%d,r%d = 0x%0.8x\n", dreg, sreg, program->vars[dreg]);
		}
		break;
	case INST_SUBIB:      // SUB reg,imm8   - reg = reg - imm
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			int8_t val = readIMM8(program);
			program->vars[reg] -= val;
			trace("SUBIB r%d = r%d - 0x%0.2X == 0x%d\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_SUBIS:      // SUB reg,imm16  - reg = reg - imm
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			int16_t val = readIMM16(program);
			program->vars[reg] -= val;
			trace("SUBIS r%d = r%d - 0x%0.4x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_SUBI:       // SUB reg,imm32  - reg = reg - imm
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			int32_t val = readIMM32(program);
			program->vars[reg] -= val;
			trace("SUBI r%d = r%d - 0x%0.8x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_SUB:        // SUB dreg,sreg  - dreg = dreg - sreg
		program->pc++;
		{
			uint8_t dreg = program->program[program->pc++]; //reg number
			uint8_t sreg = program->program[program->pc++];
			program->vars[dreg] -= program->vars[sreg];
			trace("SUB r%d = r%d - r%d == 0x%0.8x\n", dreg, dreg, sreg,
					program->vars[dreg]);
		}
		break;
	case INST_ADDIB:      // ADD reg,imm8   - reg = reg + imm
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			int8_t val = readIMM8(program);
			program->vars[reg] += val;
			trace("ADDIB r%d = r%d + 0x%0.2x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_ADDIS:      // ADD reg,imm16  - reg = reg + imm
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			int16_t val = readIMM16(program);
			program->vars[reg] += val;
			trace("ADDIS r%d = r%d + 0x%0.4x == 0x%0.8x \n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_ADDI:       // ADD reg,imm32  - reg = reg + imm
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			int32_t val = readIMM32(program);
			program->vars[reg] += val;
			trace("ADDI r%d = r%d + 0x%0.8x  == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_ADD:        // ADD dreg,sreg  - dreg = dreg + sreg
		program->pc++;
		{
			uint8_t dreg = program->program[program->pc++]; //reg number
			int8_t sreg = program->vars[program->pc++];
			program->vars[dreg] += program->vars[sreg];
			trace("ADD r%d = r%d + r%d =  0x%0.8x\n", dreg, dreg, sreg,
					program->vars[dreg]);
		}
		break;
	case INST_MULIB:      // MUL reg,imm8   - reg = reg * imm
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			int8_t val = readIMM8(program);
			program->vars[reg] *= val;
			trace("MULIB r%d = r%d * 0x%0.2x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_MULIS:      // MUL reg,imm16  - reg = reg * imm
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			int16_t val = readIMM16(program);
			program->vars[reg] *= val;
			trace("MULIS r%d = r%d * 0x%0.4x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_MULI:       // MUL reg,imm32  - reg = reg * imm
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			int32_t val = readIMM32(program);
			program->vars[reg] *= val;
			trace("MULI r%d = r%d * 0x%0.8x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_MUL:        // MUL dreg,sreg  - dreg = dreg * sreg
		program->pc++;
		{
			uint8_t dreg = program->program[program->pc++]; //reg number
			uint8_t sreg = program->program[program->pc++];
			program->vars[dreg] *= program->vars[sreg];
			trace("MUL r%d = r%d * r%d =  %0.8x\n", dreg, dreg, sreg,
					program->vars[dreg]);
		}
		break;
	case INST_DIVIB:      // MUL reg,imm8   - reg = reg / imm
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			int8_t val = readIMM8(program);
			program->vars[reg] /= val;
			trace("DIVIB r%d = r%d / 0x%0.2x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_DIVIS:      // MUL reg,imm16  - reg = reg / imm
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			int16_t val = readIMM16(program);
			program->vars[reg] /= val;
			trace("DIVIS r%d = r%d / 0x%0.4x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_DIVI:       // MUL reg,imm32  - reg = reg / imm
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			int16_t val = readIMM32(program);
			program->vars[reg] /= val;
			trace("DIVI r%d = r%d / 0x%0.8x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_DIV:        // MUL dreg,sreg  - dreg = dreg / sreg
		program->pc++;
		{
			uint8_t dreg = program->program[program->pc++]; //reg number
			uint8_t sreg = program->program[program->pc++];
			program->vars[dreg] /= program->vars[sreg];
			trace("DIV r%d = r%d * r%d =  0x%0.8x\n", dreg, dreg, sreg,
					program->vars[dreg]);
		}
		break;
	case INST_SHIFTL:     // SHIFTL dreg,imm8   dreg = dreg << imm8
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			uint8_t val = readIMM8(program);
			program->vars[reg] <<= val;
			trace("SHIFTL r%d = r%d << 0x%0.2x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_SHIFTR:     // SHIFTL dreg,imm8   dreg = dreg << imm8
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++]; //reg number
			uint8_t val = readIMM8(program);
			program->vars[reg] >>= val;
			trace("SHIFTL r%d = r%d >> 0x%0.2x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_ANDIB:      // ANDI reg,imm8     BITWISE AND
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++];
			uint8_t val = readIMM8(program);

			val = program->vars[reg] & val;
			program->vars[reg] = val;
			trace("ANDIB r%d = r%d & 0x%0.2x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_ANDIS:      // ANDI reg,imm16     BITWISE AND
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++];
			uint16_t val = readIMM16(program);

			val = program->vars[reg] & val;
			program->vars[reg] = val;
			trace("ANDIS r%d = r%d & 0x%0.4x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_ANDI:       // ANDI reg,imm32     BITWISE AND
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++];
			uint32_t val = readIMM32(program);

			val = program->vars[reg] & val;
			program->vars[reg] = val;
			trace("ANDI r%d = r%d & 0x%0.8x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_AND:        // AND  dreg,sreg  BITWISE AND dreg = dreg & sreg;
		program->pc++;
		{
			uint8_t dreg = program->program[program->pc++];
			uint8_t sreg = program->program[program->pc++];

			program->vars[dreg] = program->vars[dreg] & program->vars[sreg];
			trace("AND r%d = r%d & r%d == 0x%0.8x\n", dreg, dreg, sreg,
					program->vars[dreg]);
		}
		break;
	case INST_ORIB:       // OR reg,imm8 BITWISE OR
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++];
			uint8_t val = readIMM8(program);

			val = program->vars[reg] | val;
			program->vars[reg] = val;
			trace("ORIB r%d = r%d | 0x%0.2x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_ORIS:       // ORI reg,imm16 BITWISE OR
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++];
			uint16_t val = readIMM16(program);

			val = program->vars[reg] | val;
			program->vars[reg] = val;
			trace("ORIS r%d = r%d | 0x%0.4x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_ORI:        // ORI reg,imm32 BITWISE OR
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++];
			uint32_t val = readIMM32(program);

			val = program->vars[reg] | val;
			program->vars[reg] = val;
			trace("ORI r%d = r%d | 0x%0.8x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_OR:         // OR dreg,sreg dreg = dreg | sreg
		program->pc++;
		{
			uint8_t dreg = program->program[program->pc++];
			uint8_t sreg = program->program[program->pc++];

			program->vars[dreg] = program->vars[dreg] | program->vars[sreg];
			trace("OR r%d = r%d | r%d == 0x%0.8x\n", dreg, dreg, sreg,
					program->vars[dreg]);
		}
		break;
	case INST_XORIB:      // XOR reg,imm8 BITWISE XOR
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++];
			uint8_t val = readIMM8(program);

			program->vars[reg] = (program->vars[reg] ^ val) & 0xFF;
			trace("XORIB r%d = r%d ^ 0x%0.2x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_XORIS:      // XOR reg,imm16 BITWISE XOR
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++];
			uint16_t val = readIMM16(program);

			program->vars[reg] = (program->vars[reg] ^ val) & 0xFFFF;
			trace("XORIS r%d = r%d ^ 0x%0.4x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_XORI:       // XOR reg,imm32 BITWISE XOR
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++];
			uint32_t val = readIMM32(program);

			program->vars[reg] = program->vars[reg] ^ val;
			trace("XORI r%d = r%d ^ 0x%0.8x == 0x%0.8x\n", reg, reg, val,
					program->vars[reg]);
		}
		break;
	case INST_XOR:        // XOR dreg,sreg BITWISE XOR dreg = dreg ^ sreg
		program->pc++;
		{
			uint8_t dreg = program->program[program->pc++];
			uint8_t sreg = program->program[program->pc++];

			program->vars[dreg] = program->vars[dreg] ^ program->vars[sreg];
			trace("XOR r%d = r%d ^ r%d == 0x%0.8x\n", dreg, dreg, sreg,
					program->vars[dreg]);
		}
		break;
	case INST_NOT:        // NOT dreg   dreg = ~dreg
		program->pc++;
		{
			uint8_t reg = program->program[program->pc++];

			program->vars[reg] = ~program->vars[reg];
			trace("NOT r%d = ~r%d  == 0x%0.8x\n", reg, reg, program->vars[reg]);
		}
		break;

	case INST_JMP:
		program->pc++;
		{
			program->pc = readIMM16(program);
			trace("jmp %d\n", program->pc);
		}
		break;
	case INST_CALL:       // CALL imm  implicit PUSHS [pc], JMP imm
		program->pc++;
		{
			uint16_t newAddr = readIMM16(program);
			uint16_t value = program->pc;
			program->stack[program->sp++] = (uint32_t)(value & 0xFFFF);
			program->pc = newAddr;
			trace("call %d (ret addr= %d)\n", program->pc, value);

			program->pc = newAddr;
		}
		break;
	case INST_RET:  // return to address on top of stack implicit pop imm --> pc
		program->pc++;
		{
			program->pc = (uint16_t) program->stack[--program->sp] & 0xFFFF;
			trace("ret %d\n", program->pc);
		}
		break;
	case INST_CMPIB: // cmp reg,imm8  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg [=,>,<] imm8
		program->pc++;
		{
			uint16_t reg = program->program[program->pc++];
			int8_t val = readIMM8(program);

			if (program->vars[reg] == val)
				program->flags = FLAG_EQ;
			else if (program->vars[reg] > val)
				program->flags = FLAG_GT;
			else
				program->flags = FLAG_LT;

			trace("cmpib r%d,0x%0.2X == %d", reg, val, program->flags);
		}
		break;
	case INST_CMPIS: // cmp reg,imm16  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg [=,>,<] imm16
		program->pc++;
		{
			uint16_t reg = program->program[program->pc++];
			int16_t val = readIMM16(program);

			if (program->vars[reg] == val)
				program->flags = FLAG_EQ;
			else if (program->vars[reg] > val)
				program->flags = FLAG_GT;
			else
				program->flags = FLAG_LT;

			trace("cmpis r%d,0x%0.4X == %d", reg, val, program->flags);
		}
		break;
	case INST_CMPI: // cmp reg,imm32  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg [=,>,<] imm32
		program->pc++;
		{
			uint16_t reg = program->program[program->pc++];
			int32_t val = readIMM32(program);

			if (program->vars[reg] == val)
				program->flags = FLAG_EQ;
			else if (program->vars[reg] > val)
				program->flags = FLAG_GT;
			else
				program->flags = FLAG_LT;

			trace("cmpi r%d,0x%0.8X == %d", reg, val, program->flags);
		}
		break;
	case INST_CMP: // cmp reg1,reg2  = sets FLAG to FLAG_EQ,FLAG_GT,FLAG_LT based on reg1 [=,>,<] reg2
		program->pc++;
		{
			uint16_t reg = program->program[program->pc++];
			uint8_t reg2 = program->program[program->pc++];

			if (program->vars[reg] == program->vars[reg2])
				program->flags = FLAG_EQ;
			else if (program->vars[reg] > program->vars[reg2])
				program->flags = FLAG_GT;
			else
				program->flags = FLAG_LT;

			trace("cmp r%d,r%d == %d", reg, reg2, program->flags);
		}
		break;
	case INST_JEQ:        // Jump addr - jump to address if FLAG = FLAG_EQ
		program->pc++;
		{
			uint16_t addr = readIMM16(program);
			if (program->flags == FLAG_EQ)
				program->pc = addr;

			trace("jeq 0x%.4X  pc == 0x%0.4X", addr, program->pc);
		}
		break;
	case INST_JNE:        // Jump addr - jump to address if FLAG != FLAG_EQ
		program->pc++;
		{
			uint16_t addr = readIMM16(program);
			if (program->flags != FLAG_EQ)
				program->pc = addr;

			trace("jne 0x%.4X  pc == 0x%0.4X", addr, program->pc);
		}
		break;
	case INST_JLT:        // Jump addr - jump to address if FLAG = FLAG_LT
		program->pc++;
		{
			uint16_t addr = readIMM16(program);
			if (program->flags == FLAG_LT)
				program->pc = addr;

			trace("jlt 0x%.4X  pc == 0x%0.4X", addr, program->pc);
		}
		break;
	case INST_JGT:        // Jump addr - jump to address if FLAG = FLAG_0
		program->pc++;
		{
			uint16_t addr = readIMM16(program);
			if (program->flags == FLAG_GT)
				program->pc = addr;

			trace("jgt 0x%.4X  pc == 0x%0.4X", addr, program->pc);
		}
		break;

	case INST_PUSHCR:
		program->pc++;
		{
			uint8_t stripNum = program->program[program->pc++];
			uint8_t reg = program->program[program->pc++];

			trace("pushcr stripnum[%d] Color(%.8x)\n", stripNum,
					program->vars[reg]);
			exec_INST_ROTATEO(program, stripNum);
			setPixelColor(&program->strips[stripNum], 0,
					((program->vars[reg] >> 16) & 0xff),
					((program->vars[reg] >> 8) & 0xff),
					(program->vars[reg] & 0xff));
		}

		break;
	case INST_PULLCR: // imm8,reg - pull a pixel into head unit and shift in from end of strip (reg = xrgb)
		program->pc++;
		{
			uint8_t stripNum = program->program[program->pc++];
			uint8_t reg = program->program[program->pc++];

			trace("pullcr stripnum[%d] Color(%.8x)\n", stripNum,
					program->vars[reg]);
			exec_INST_ROTATEI(program, stripNum);
			setPixelColor(&program->strips[stripNum],
					program->strips[stripNum].count - 1,
					((program->vars[reg] >> 16) & 0xff),
					((program->vars[reg] >> 8) & 0xff),
					(program->vars[reg] & 0xff));
		}
		break;
	case INST_ROTATEO:
		program->pc++;
		exec_INST_ROTATEO(program, program->program[program->pc++]);
		break;

	case INST_ROTATEI: // imm8     - ledStrip# rotate 1 leds into head unit and those colors shifted in goto end of strip
		program->pc++;
		{
			exec_INST_ROTATEI(program, program->program[program->pc++]);
		}
		break;
	case INST_SETPIXEL: // imm8,reg1,reg2 - For LED strip imm8 SET LED=reg1 to to xrgb color = reg2
		program->pc++;
		{
			uint8_t num = readIMM8(program);
			uint8_t reg1 = program->program[program->pc++];
			uint8_t reg2 = program->program[program->pc++];

			trace("Set pixel %d,%d,%d\n", num, reg1, reg2);

			setPixelColor(&program->strips[num],program->vars[reg1],((program->vars[reg2] >> 16) & 0xff),((program->vars[reg2] >> 8) & 0xff),(program->vars[reg2] & 0xff));
		}
		break;
	case INST_READPIXEL: // imm8,reg1,reg2 - For LED strip imm8 READ LED=reg1 into reg2
		program->pc++;
		{
			uint8_t num = readIMM8(program);
			uint8_t dreg = program->program[program->pc++];
			uint8_t sreg = program->program[program->pc++];

			program->vars[dreg] = getPixelColor(&program->strips[num], program->vars[sreg]);
		}
		break;
	case INST_UPDATE:
		program->pc++;
		{
			uint8_t num = readIMM8(program);
			program->strips[num].sync = 1;
		}
		break;
	default:

		error("Invalid program instruction %d\n",
				program->program[program->pc]);
		program->state = STATE_IDLE;
		break;
	}
}

float readFloat(LEDProgram* program) {
	uint32_t value = *((uint32_t *) (&program->program[program->pc]));

	value = SWAP32(value);
	program->pc += 4;

	return *((float *) (&value));;
}

int8_t readIMM8(LEDProgram* program) {
	return (int8_t) program->program[program->pc++];
}
int16_t readIMM16(LEDProgram* program) {
	int16_t value = program->program[program->pc++];
	value <<= 8;
	value += program->program[program->pc++];

#ifndef BIGENDIAN
	value = SWAP16(value);
#endif

	return value;
}
int32_t readIMM32(LEDProgram* program) {
	int32_t value = program->program[program->pc++];
	value <<= 8;
	value += program->program[program->pc++];
	value <<= 8;
	value += program->program[program->pc++];
	value <<= 8;
	value += program->program[program->pc++];

#ifndef BIGENDIAN
	value = SWAP32(value);
#endif

	return value;
}

void setPixelColor(LEDStrip* strip, uint16_t pixel, uint8_t r, uint8_t g, uint8_t b) {
	if (strip->pixels == NULL) {
		error("strip[%d] not initialized", ledNum);
		return;
	}
	uint16_t offset = pixel * 3;
	strip->pixels[offset] = g * strip->brightness >> 8;
	strip->pixels[offset + 1] = r * strip->brightness >> 8;
	strip->pixels[offset + 2] = b * strip->brightness >> 8;
}

uint32_t getPixelColor(LEDStrip* strip, uint16_t pixel) {
	uint32_t rv = 0;

	uint16_t offset = pixel * 3;
	rv = strip->pixels[offset + 1];
	rv <<= 8;
	rv += strip->pixels[offset];
	rv <<= 8;
	rv += strip->pixels[offset + 2];

	return rv;
}

void exec_INST_ROTATEO(LEDProgram* program, uint8_t stripNum) {

	LEDStrip* strip = &program->strips[stripNum];
	trace("executing INST_ROTATEO stripNum[%d]\n", stripNum);
	uint8_t *p = strip->pixels + strip->count * 3 - 1;
	uint8_t b = *p;
	uint8_t r = *(p - 1);
	uint8_t g = *(p - 2);
	for (; p > strip->pixels + 3;) {
		*p = *(p - 3);
		p--;
		*p = *(p - 3);
		p--;
		*p = *(p - 3);
		p--;
	}

	*strip->pixels = r;
	*(strip->pixels + 1) = g;
	*(strip->pixels + 2) = b;
	setPixelColor(strip,0, r, g, b);

}
void exec_INST_ROTATEI(LEDProgram* program, uint8_t stripNum) {

	LEDStrip* strip = &program->strips[stripNum];

	uint8_t r = *strip->pixels;
	uint8_t g = *(strip->pixels + 1);
	uint8_t b = *(strip->pixels + 2);
	trace("executing INST_ROTATEI stripNum[%d]\n", stripNum);
	uint8_t* pend = strip->pixels + strip->count * 3;
	uint8_t* p;
	for (p = strip->pixels; p < pend;) {
		*p = *(p + 3);
		p++;
		*p = *(p + 3);
		p++;
		*p = *(p + 3);
		p++;
	}
	*(pend - 3) = r;
	*(pend - 2) = g;
	*(pend - 1) = b;
}
