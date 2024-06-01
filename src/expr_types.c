//
// Created by Alex on 25.05.2024.
//

#include "expr_types.h"

void print_token(token_t token) {
  switch (token.type) {
    case TOKEN_OPERATOR:
      printf("%c\n", token.data.operator);
    break;
    case TOKEN_VARIABLE:
      printf("%s\n", token.data.var_name);
    break;
    case TOKEN_CONST:
      printf("%lf\n", token.data.const_var);
    break;
    case TOKEN_LEFT_BRACKET:
      printf("left bracket\n");
    break;
    case TOKEN_RIGHT_BRACKET:
      printf("right bracket\n");
    break;
    default:
      printf("shit\n");
    break;
  }
}