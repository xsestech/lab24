//
// Created by Alex on 25.05.2024.
//

#ifndef BTREE_H
#define BTREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "expr_types.h"
#include "list.h"


typedef struct tree_node_t tree_node_t;
typedef tree_node_t* tree_t;

#define STACK_TYPE tree_t
#include "stack.h"

struct tree_node_t {
  token_t value;
  tree_node_t *left;
  tree_node_t *right;
};

/**
 * Initializes empty tree
 * @return Empty tree
 */
tree_t tree_init();
/**
 * Destroys all tree nodes
 * @param tree pointer to tree to destroy
 */
void tree_destroy(tree_t* tree);

/**
 * Get token from node
 * @param node pointer to node
 * @return Token
 */
token_t tree_get_value(tree_node_t *node);

/**
 * Print binary tree
 * @param tree tree handle
 */
void tree_print(tree_t tree);
tree_t tree_build_from_expr(list_handle_t postfix_expr);

#endif //BTREE_H
