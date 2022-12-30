#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "./stack.h"
#include "./exec.h"

#define MEMORY_LEN 32768
#define NUM_REGISTERS 8

int load(uint16_t *memory, int length) {
	// load challenge.bin in the memory array
	FILE *fp;

	if ((fp = fopen("challenge.bin", "rb")) == NULL) {
		return -1;
	} else {	
		fread(memory, length, 2, fp);
	}
	fclose(fp);
	return 0;
}

int main(int argc, char** argv) {

	uint16_t memory[MEMORY_LEN];
	uint16_t registers[NUM_REGISTERS];
  stack *stack = new_stack();


	if (load(memory, MEMORY_LEN) == 0) {
		printf("\nProgram successfully loaded.\n");
	} else {
		printf("Error: program not loaded.\n");
    return 1;
	}

  int cursor = 0;
  while (cursor != MEMORY_LEN && cursor != -1) {
    cursor = read(cursor, memory, registers, stack);
  }
	return 0;
}

