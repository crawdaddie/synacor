#include "./exec.h"
#include "./stack.h"
enum OPCODES {
  HALT,
  SET,
  PUSH,
  POP,
  EQ,
  GT,
  JMP,
  JT,
  JF,
  ADD,
  MULT,
  MOD,
  AND,
  OR,
  NOT,
  RMEM,
  WMEM,
  CALL,
  RET,
  OUT,
  IN,
  NOOP,
};
 
uint16_t readvar(uint16_t var, uint16_t *registers) {
	// return the value of an argument (stored in a register or hard-coded in the memory)
	if (var < 32768) {
		return var;
	}
	return registers[var % 32768];
}

int read(int cursor, uint16_t *memory, uint16_t *registers, stack *s) {
  uint16_t test = 0;
  uint16_t mem_val = memory[cursor];
  uint16_t a, b, c;
  printf("%d | %x\n", cursor, mem_val);
  switch (mem_val) {
  case HALT: //: 0 stop execution and terminate the program
    return -1;
    

  case SET: //: 1 a b set register <a> to the value of <b>
      cursor++;
      a = memory[cursor+1] % 32768;
      b = memory[cursor+2];
      registers[a] = readvar(b, registers);
      return cursor+3;
  case PUSH: {
      // 2 a push <a> onto the stack
      uint16_t val = readvar(memory[cursor+1], registers);
      stack_push(s, val);

      return cursor+2;
    }
      

  case POP: //: 3 a remove the top element from the stack and write it into <a>; empty stack = error
    if (stack_is_empty(s)) {
      return -1;
    }

    uint16_t top = stack_pop(s);
    printf("popped %x\n", top);

    a = memory[cursor + 1] % 32768;
    registers[a] = top;
    return cursor+2;
    
  case EQ:
    //: 4 a b c set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise
    a = memory[cursor+1] % 32768;
    b = memory[cursor+2];
    c = memory[cursor+3];
    printf("EQ b(%x) == c(%x)\n", readvar(b, registers), readvar(c, registers));

    registers[a] = readvar(b, registers) == readvar(c, registers) ? 1 : 0;
    return cursor+4;
    

  case GT:
    //: 5 a b c set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise

    a = memory[cursor+1] % 32768;
    b = memory[cursor+2];
    c = memory[cursor+3];
    registers[a] = readvar(b, registers) > readvar(c, registers) ? 1 : 0;
    return cursor+4;
    

  case JMP:

    // : 6 a jump to <a>
    a = memory[cursor+1];
    return readvar(a, registers);
    
 

  case JT:
    // : 7 a b if <a> is nonzero, jump to <b>
    a = memory[cursor+1];
    b = memory[cursor+2];
    if (readvar(a, registers) != 0) {
      return readvar(b, registers);
    }
    return cursor+3;

  case JF:

    //: 8 a b if <a> is zero, jump to <b>
    a = memory[cursor+1];
    b = memory[cursor+2];
    if (readvar(a, registers) == 0) {
      return readvar(b, registers);
    } else {
      return cursor+3;
    }

    

  case ADD: 
    //: 9 a b c assign into <a> the sum of <b> and <c> (modulo 32768)
    a = memory[cursor+1] % 32768;
    b = memory[cursor+2];
    c = memory[cursor+3];
    registers[a] = (readvar(b, registers) + readvar(c, registers)) % 32768;
    return cursor+4;
    

  case MULT: 

    //: 10 a b c store into <a> the product of <b> and <c> (modulo 32768)
    a = memory[cursor+1] % 32768;
    b = memory[cursor+2];
    c = memory[cursor+3];
    registers[a] = (readvar(b, registers) * readvar(c, registers)) % 32768;
    return cursor+4;
    

  case MOD:
    //: 11 a b c store into <a> the remainder of <b> divided by <c>
    a = memory[cursor+1] % 32768;
    b = memory[cursor+2];
    c = memory[cursor+3];
    registers[a] = readvar(b, registers) % readvar(c, registers);
    return cursor+4;
    

  case AND: //: 12 a b c stores into <a> the bitwise and of <b> and <c>
    a = memory[cursor+1] % 32768;
    b = memory[cursor+2];
    c = memory[cursor+3];
    registers[a] = readvar(b, registers) & readvar(c, registers);
    return cursor+4;
    

  case OR: 
    //: 13 a b c stores into <a> the bitwise or of <b> and <c>
    a = memory[cursor+1] % 32768;
    b = memory[cursor+2];
    c = memory[cursor+3];
    registers[a] = readvar(b, registers) | readvar(c, registers);
    return cursor+4;

  case NOT: //: 14 a b stores 15-bit bitwise inverse of <b> in <a>
    a = memory[cursor+1] % 32768;
    b = memory[cursor+2];
    registers[a] = ~ readvar(b, registers) & ((1 << 15) - 1);
    return cursor+3;

  case RMEM:
    //: 15 a b read memory at address <b> and write it to <a>
    a = memory[cursor+1] % 32768;
    b = memory[cursor+2];
    registers[a] = memory[readvar(b, registers)];
    return cursor+3;

  case WMEM:
    a = memory[cursor+1];
    b = memory[cursor+2];
    memory[readvar(a, registers)] = readvar(b, registers);
    return cursor+3;
    //: 16 a b write the value from <b> into memory at address <a>

  case CALL: //: 17 a write the address of the next instruction to the stack and jump to <a>
    a = memory[cursor+1];
    b = cursor + 2;
    stack_push(s, b);

	  printf("%x memval(%x)| call: push(%x) go to memory[%x] (%x)\n", cursor, mem_val, b, readvar(a, registers), memory[readvar(a, registers)]);
    return readvar(a, registers);

  case RET: //: 18 remove the top element from the stack and jump to it; empty stack = halt
    //
    printf("stack ret\n");

    if (stack_is_empty(s)) {
      return -1;
    }
    return stack_pop(s);

  case OUT:
    a = memory[cursor+1] % 32768;
    //: 19 a write the character represented by ascii code <a> to the terminal
    printf("%c", a);
    return cursor+2;
     

  case IN:
    a = memory[cursor+1] % 32768;
    char c = getchar();
    registers[a] = c;
    return cursor+2;
    //: 20 a
    // read a character from the terminal and write its ascii code to <a>;
    // it can be assumed that once input starts:
    // it will continue until a newline is encountered;
    // this means that you can safely read whole lines from the keyboard and trust that they will be fully read

  case NOOP:
    return cursor+1;
  //: 21
  default:
    return cursor+1;
  }
  return cursor+1;
}


