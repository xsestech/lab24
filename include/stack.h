//
// Created by Alex on 21.04.2024.
//

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include "btree.h"

#ifndef STACK_TYPE
#define STACK_TYPE double
#endif

typedef struct stack_meta_t stack_meta_t;
typedef stack_meta_t* stack_handle_t;

// #define STACK_TYPE tree_t*

/**
 * Initializes empty stack
 * @return empty stack pointer
 */
stack_handle_t stack_init();
/**
 * Destroys stack
 * @param stack pointer to stack to destroy
 */
void stack_destroy(stack_handle_t stack);
/**
 * Pushes value to stack
 * @param stack stack pointer
 * @param value value to push
 */
void stack_push(stack_handle_t stack, STACK_TYPE value);
/**
 * Pops value from stack
 * @param stack pointer to stack to push to
 * @return value that was popped
 */
STACK_TYPE stack_pop(stack_handle_t stack);
/**
 * Checks stack emptiness
 * @param stack stack pointer
 * @return true is stack is empty
 */
bool stack_is_empty(stack_handle_t stack);
#endif //STACK_H
