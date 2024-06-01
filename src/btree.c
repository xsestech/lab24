//
// Created by Alex on 25.05.2024.
//

#include "btree.h"

#include <string.h>

#define swap(a, b, type) \
  do {                   \
  type tmp__ = a;        \
  a = b;                 \
  b = tmp__;             \
  } while (0);

#define swap_nodes(a, b) \
  swap(a, b, tree_t)

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

bool tree_op_cmp(tree_t tree, char op) {
  if (tree == NULL) {
    return false;
  }
  return (tree->value.data.operator == op) && (tree->value.type == TOKEN_OPERATOR);
}

tree_t tree_deepest_operator(tree_t tree, char operator) {
  if (!tree_op_cmp(tree, operator)) {
    return NULL;
  }
  stack_handle_t stack = stack_init();
  stack_push(stack, tree);
  tree_t deepest_node = tree;
  int depth = 0;
  while(!stack_is_empty(stack)) {
    tree_t node = stack_pop(stack);
    if (tree_op_cmp(node->right, operator)) {
      stack_push(stack, node->right);
      deepest_node = node->right;
    }
    if (tree_op_cmp(node->left, operator)) {
      stack_push(stack, node->left);
      deepest_node = node->left;
    }
    depth++;
  }
  if (depth == 1) {
    return NULL;
  }
  stack_destroy(stack);
  return deepest_node;
}

// tree_t tree_operator_sort(tree_t tree, char operator) {
//   if (!tree_op_cmp(tree, operator)) {
//     return NULL;
//   }
//   stack_handle_t stack = stack_init();
//   stack_push(stack, tree);
//   while(!stack_is_empty(stack)) {
//     tree_t node = stack_pop(stack);
//     if (tree_op_cmp(node->right, operator)) {
//       stack_push(stack, node->right);
//     }
//     if (tree_op_cmp(node->left, operator)) {
//       stack_push(stack, node->left);
//     }
//   }
// }

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
      // if ((right->value.type == TOKEN_OPERATOR) && (right->value.data.operator == token.data.operator)) {
      //   tree_t deepest_op = tree_deepest_operator(left, right->value.data.operator);
      //   if(deepest_op != NULL) {
      //     swap_nodes(deepest_op->left, right);
      //   } else {
      //     swap_nodes(left, right);
      //   }
      // }

      tree_node_t *new_node = tree_malloc_node(token, left, right);
      stack_push(nodes, new_node);
    }
  }
  return stack_pop(nodes);
}



int tree_depth_recursive(tree_t tree, int depth) {
  if (tree == NULL) {
    return depth;
  }
  int left_depth = tree_depth_recursive(tree->left, depth + 1);
  int right_depth = tree_depth_recursive(tree->right, depth + 1);
  return left_depth > right_depth ? left_depth : right_depth;
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

int tree_depth(tree_t tree) {
  return tree_depth_recursive(tree, 0);
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



void tree_print(tree_t tree) {
  tree_print_recursive(tree, 0);
}