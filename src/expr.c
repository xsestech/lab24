//
// Created by Alex on 25.05.2024.
//

#include "expr.h"

const char operators[] = {'+', '*', '/'};

bool is_operator(char ch) {
  for (int i = 0; i < sizeof(operators); i++) {
    if (ch == operators[i]) {
      return true;
    }
  }
  return false;
}

int operator_priority(char op) {
  switch (op) {
    case '~':
      return 3;
    case '*':
    case '/':
      return 2;
    case '+':
    case '-':
      return 1;
    default:
      return 0;
  }
}

bool is_operator_left_associative(char op) {
  return (op != '~');
}

read_token_result_t read_token(token_t *out, token_type_t prev) {
  char input = getc(stdin);
  if (input == EOF)
    return READ_TOKEN_EOF;
  if (input == '\n')
    return READ_TOKEN_EOL;
  while (input == ' ' || input == '\t') {
    input = getc(stdin);
  }
  if (isalpha(input)) {
    memset(out->data.var_name, 0,EXPR_MAX_VARIABLE_NAME_LEN);
    for (int i = 0; (isalpha(input) || isdigit(input) || input == '-') && i < EXPR_MAX_VARIABLE_NAME_LEN; i++) {
      out->data.var_name[i] = input;
      input = getc(stdin);
    }
    ungetc(input, stdin);
    out->type = TOKEN_VARIABLE;
    return READ_TOKEN_SUCCESS;
  }
  if (isdigit(input)) {
    ungetc(input, stdin);
    double number;
    if (!scanf("%lf", &number)) {
      return READ_TOKEN_INVALID_CHAR;
    }
    out->data.const_var = number;
    out->type = TOKEN_CONST;
    return READ_TOKEN_SUCCESS;
  }
  if (input == EOF) {
    return READ_TOKEN_EOF;
  }
  if (input == '\n') {
    return READ_TOKEN_EOL;
  }
  if (input == '(') {
    out->type = TOKEN_LEFT_BRACKET;
    return READ_TOKEN_SUCCESS;
  }
  if (input == ')') {
    out->type = TOKEN_RIGHT_BRACKET;
    return READ_TOKEN_SUCCESS;
  }
  if (input == '-') {
    out->type = TOKEN_OPERATOR;
    if (prev == TOKEN_LEFT_BRACKET || prev == TOKEN_NONE || prev == TOKEN_OPERATOR) {
      out->data.operator = '~';
      return READ_TOKEN_SUCCESS;
    }
    out->data.operator = '-';
    return READ_TOKEN_SUCCESS;
  }
  if (is_operator(input)) {
    out->type = TOKEN_OPERATOR;
    out->data.operator = input;
    return READ_TOKEN_SUCCESS;
  }
  return READ_TOKEN_UNEXPECTED_INPUT;
}

read_token_result_t read_expr(list_handle_t list) {
  token_t a;
  token_type_t prev = TOKEN_NONE;
  read_token_result_t read_status = read_token(&a, prev);
  while (read_status != READ_TOKEN_EOF && read_status != READ_TOKEN_EOL) {
    if (read_status == READ_TOKEN_SUCCESS) {
      list_push_back(list, a);
      prev = a.type;
    } else {
      return read_status;
    }
    read_status = read_token(&a, prev);
  }
  if (list_is_empty(list)) {
    return READ_EXPR_EMPTY;
  }
  return READ_EXPR_SUCCESS;
}



bool shunting_yard(list_handle_t infinx_expr, list_handle_t postfix_expr) {
  list_handle_t stack = list_init();
  list_iter_t iter = list_begin(infinx_expr);
  while (!list_iter_equal(iter, list_iter_null(infinx_expr))) {
    token_t token = list_iter_val(iter);
    token_t top_token;
    switch (token.type) {
      case TOKEN_OPERATOR:
        if (!list_is_empty(stack)) {
          top_token = list_iter_val(list_end(stack));
          while ((top_token.type == TOKEN_OPERATOR) && ((operator_priority(top_token.data.operator) >
            operator_priority(token.data.operator)) || (operator_priority(top_token.data.operator) ==
            operator_priority(token.data.operator) && is_operator_left_associative(token.data.operator)))) {
            token_t op = list_remove(list_end(stack));
            list_push_back(postfix_expr, op);
          }
        }
        list_push_back(stack, token);
        break;
      case TOKEN_CONST:
      case TOKEN_VARIABLE:
        list_push_back(postfix_expr, token);
        break;
      case TOKEN_LEFT_BRACKET:
        list_push_back(stack, token);
        break;
      case TOKEN_RIGHT_BRACKET:
        top_token = list_iter_val(list_end(stack));
        while (top_token.type != TOKEN_LEFT_BRACKET) {
          if (list_is_empty(stack)) {
            return false;
          }
          list_push_back(postfix_expr, top_token);
          list_remove(list_end(stack));
          top_token = list_iter_val(list_end(stack));
        }
        if (list_iter_val(list_end(stack)).type != TOKEN_LEFT_BRACKET) {
          return false;
        }
        list_remove(list_end(stack));
        break;
      default:
        return false;
    }
    iter = list_iter_next(iter);
  }
  while (!list_is_empty(stack)) {
    if (list_iter_val(list_end(stack)).type == TOKEN_LEFT_BRACKET) {
      return false;
    }
    list_push_back(postfix_expr, list_remove(list_end(stack)));
  }
  list_destroy(stack);

  return true;
}
