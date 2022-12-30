#ifndef _EXEC_H
#define _EXEC_H
#include <stdint.h>
#include "./stack.h"
uint16_t readvar(uint16_t var, uint16_t *registers);
int read(int cursor, uint16_t *memory, uint16_t *registers, stack *stack);
#endif
