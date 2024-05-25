//
// Created by Alex on 25.05.2024.
//

#include "btree.h"



tree_t tree_init() {
  return NULL;
}

tree_t tree_malloc_node(token_t token, tree_node_t* left, tree_node_t* right) {
  tree_node_t* node = malloc(sizeof(tree_node_t));
  node->value = token;
  node->left = left;
  node->right = right;
  return node;
}

tree_t tree_build_from_expr(list_handle_t postfix_expr) {
  stack_handle_t nodes = stack_init();
  for (list_iter_t iter = list_begin(postfix_expr); !list_iter_equal(iter, list_iter_null(postfix_expr)); iter = list_iter_next(iter)) {
    token_t token = list_iter_val(iter);
    if (token.type != TOKEN_OPERATOR) {
      stack_push(nodes, tree_malloc_node(token, NULL, NULL));
    } else {
      tree_node_t* left = stack_pop(nodes);
      tree_node_t* right = NULL;
      if (token.data.operator != '~') {
        right = stack_pop(nodes);
      }
      tree_node_t* new_node = tree_malloc_node(token, left, right);
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

void tree_print(tree_t tree) {
  tree_print_recursive(tree, 0);
}
