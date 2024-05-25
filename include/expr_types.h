//
// Created by Alex on 25.05.2024.
//

#ifndef EXPR_TYPES_H
#define EXPR_TYPES_H

#include <stdio.h>

#ifndef EXPR_MAX_VARIABLE_NAME_LEN
#define EXPR_MAX_VARIABLE_NAME_LEN 50
#endif

typedef enum {
  READ_TOKEN_SUCCESS,
  READ_TOKEN_EOF,
  READ_TOKEN_EOL,
  READ_TOKEN_INVALID_CHAR,
  READ_TOKEN_UNEXPECTED_INPUT,
  READ_EXPR_SUCCESS,
  READ_EXPR_EMPTY,
} read_token_result_t;

typedef enum {
  TOKEN_NONE,
  TOKEN_VARIABLE,
  TOKEN_CONST,
  TOKEN_LEFT_BRACKET,
  TOKEN_RIGHT_BRACKET,
  TOKEN_OPERATOR
} token_type_t;

typedef struct {
  union {
    char var_name[EXPR_MAX_VARIABLE_NAME_LEN];
    double const_var;
    char operator;
  } data;
  token_type_t type;
} token_t;

void print_token(token_t token);
#endif //EXPR_TYPES_H
