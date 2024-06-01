//
// Created by Alex on 25.05.2024.
//

#include "btree.h"

#include <string.h>

tree_t tree_init() {
  return NULL;
}

tree_t tree_malloc_node(token_t token, tree_node_t *left, tree_node_t *right) {
  tree_node_t *node = malloc(sizeof(tree_node_t));
  node->value = token;
  node->left = left;
  node->right = right;
  return node;
}

tree_t tree_build_from_expr(list_handle_t postfix_expr) {
  stack_handle_t nodes = stack_init();
  for (list_iter_t iter = list_begin(postfix_expr); !list_iter_equal(iter, list_iter_null(postfix_expr)); iter =
       list_iter_next(iter)) {
    token_t token = list_iter_val(iter);
    if (token.type != TOKEN_OPERATOR) {
      stack_push(nodes, tree_malloc_node(token, NULL, NULL));
    } else {
      tree_node_t *right = stack_pop(nodes);
      tree_node_t *left = NULL;
      if (token.data.operator != '~') {
        left = stack_pop(nodes);
      } else {
        token_t zero;
        zero.type = TOKEN_CONST;
        zero.data.const_var = 0;
        left = tree_malloc_node(zero, NULL, NULL);
        token.data.operator = '-';
      }
      tree_node_t *new_node = tree_malloc_node(token, left, right);
      stack_push(nodes, new_node);
    }
  }
  return stack_pop(nodes);
}

void tree_print_recursive(tree_t tree, int h) {
  if (tree == NULL) {
    return;
  }
  for (int i = 0; i < h; i++) {
    printf("\t");
  }
  print_token(tree->value);
  printf("\n");
  tree_print_recursive(tree->left, h + 1);
  tree_print_recursive(tree->right, h + 1);
}

bool token_cmp(token_t token1, token_t token2) {
  if (token1.type != token2.type) {
    return false;
  }
  switch (token1.type) {
    case TOKEN_VARIABLE:
      return strcmp(token1.data.var_name, token2.data.var_name) == 0;
    case TOKEN_CONST:
      return token1.data.const_var == token2.data.const_var;
    case TOKEN_OPERATOR:
      return token1.data.operator == token2.data.operator;
    default:
      return false;
  }
}

tree_t dfs(tree_t tree, token_t token) {
  stack_handle_t stack = stack_init();
  stack_push(stack, tree);
  while (!stack_is_empty(stack)) {
    tree_t tree_tmp = stack_pop(stack);
    if (tree_tmp->left != NULL) {
      stack_push(stack, tree_tmp->left);
    }
    if (tree_tmp->right != NULL) {
      stack_push(stack, tree_tmp->right);
    }
    if (token_cmp(tree->value, token)) {
      return tree_tmp;
    }
  }
  stack_destroy(stack);
  return NULL;
}

void tree_compare(tree_t tree1, tree_t tree2) {
  while (tree1->value.type != TOKEN_CONST && tree1->value.type != TOKEN_VARIABLE) {
    tree1 = tree1->left;
  }
  tree_t similar_node = dfs(tree2, tree1->value);

}

void tree_print(tree_t tree) {
  tree_print_recursive(tree, 0);
}
