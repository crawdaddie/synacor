#ifndef _STACK_H
#define _STACK_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define STACK_MAX 256

typedef struct stack {
  uint16_t *items;
  size_t length;
  size_t MAX;
} stack;

stack *new_stack();
void stack_push(stack *s, uint16_t item);
int stack_is_empty(stack *s);
uint16_t stack_pop(stack *s);
#endif
