//
// Created by Alex on 25.05.2024.
//

#ifndef EXPR_H
#define EXPR_H

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"

#include "expr_types.h"



read_token_result_t read_token(token_t *out, token_type_t prev);
read_token_result_t read_expr(list_handle_t list);
void print_token(token_t token);
bool shunting_yard(list_handle_t infinx_expr, list_handle_t postfix_expr);
void expr_check_error(read_token_result_t error);

#endif //EXPR_H
