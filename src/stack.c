#include "./stack.h"

stack *new_stack() {
  uint16_t *items = malloc(sizeof(uint16_t) * STACK_MAX);
  stack *stack = malloc(sizeof(stack));
  stack->items = items;
  stack->length = 0;
  stack->MAX = STACK_MAX;
  return stack;
}

void stack_push(stack *s, uint16_t item) {
  if (s->length == s->MAX - 1) {
    s->items = (uint16_t *)realloc(s->items, sizeof(uint16_t) * (s->MAX + STACK_MAX));
    s->MAX += STACK_MAX;
  }
  s->items[s->length] = item;
  s->length += 1;
}

int stack_is_empty(stack *s) {
  return s->length == 0;
}

uint16_t stack_pop(stack *s) {
  if (stack_is_empty(s)) {
    return -1;
  }
  uint16_t item = s->items[s->length - 1];
  s->length -= 1;
  return item;
}
