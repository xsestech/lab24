//
// Created by Alex on 21.04.2024.
//
#include "stack.h"



struct stack_meta_t {
  size_t size;
  size_t capacity;
  STACK_TYPE *data;
};

stack_handle_t stack_init() {
  stack_handle_t stack = malloc(sizeof(stack_meta_t));
  stack->capacity = 0;;
  stack->size = 0;
  stack->data = NULL;
  return stack;
}

void stack_destroy(stack_handle_t stack) {
  free(stack->data);
  free(stack);
}
/**
 * Logariphmic stack growth
 * @param old_capacity capacity, that needs to be increased
 * @return new capacity
 * @internal
 */
size_t stack_new_capacity(size_t old_capacity) {
  if (!old_capacity) {
    return 1;
  }
  return old_capacity << 1;
}
/**
 * Increases stack size to new capacity
 * @param stack pointer to stack
 * @param new_capacity new capacity
 * @internal
 */
void stack_grow(stack_handle_t stack, size_t new_capacity) {
  size_t new_data_size = new_capacity * sizeof(*(stack->data));
  if (stack->data) {
    stack->data = realloc(stack->data, new_data_size);
  } else {
    stack->data = malloc(new_data_size);
  }
  assert(stack->data);
  stack->capacity = new_capacity;
}

void stack_push(stack_handle_t stack, STACK_TYPE value) {
  if (stack->capacity <= stack->size) {
    stack_grow(stack, stack_new_capacity(stack->size));
  }
  (stack->data)[stack->size] = value;
  stack->size++;
}

bool stack_is_empty(stack_handle_t stack) {
  return stack->size == 0;
}

STACK_TYPE stack_pop(stack_handle_t stack) {
  if (stack_is_empty(stack)) {
    return NULL;
  }
  return stack->data[--stack->size];
}
