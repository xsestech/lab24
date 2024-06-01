#include "expr.h"
#include "btree.h"

int main(void) {
  list_handle_t infix_expr = list_init();
  read_token_result_t error = read_expr(infix_expr);
  expr_check_error(error);
  printf("Infix:\n");
  for (list_iter_t iter = list_begin(infix_expr); !list_iter_equal(iter, list_iter_null(infix_expr)); iter = list_iter_next(iter)) {
    print_token(list_iter_val(iter));
  }
  list_handle_t postfix_expr = list_init();

  if(!shunting_yard(infix_expr, postfix_expr)) {
    fprintf(stderr, "Invalid brackets");
    return 1;
  }
  printf("Postfix:\n");
  for (list_iter_t iter = list_begin(postfix_expr); !list_iter_equal(iter, list_iter_null(postfix_expr)); iter = list_iter_next(iter)) {
    print_token(list_iter_val(iter));
  }
  printf("Tree:\n");
  tree_t tree = tree_build_from_expr(postfix_expr);
  tree_print(tree);
  list_destroy(infix_expr);
  printf("Tree depth: %d", tree_depth(tree));
  return 0;
}
