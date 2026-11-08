#ifndef PRETTY_PRINT_H
#define PRETTY_PRINT_H

#include "../ast/ast.h"

/* Pretty-print an AST node to stdout with the given indentation level */
void pretty_print(const struct ast_node *node, int indent);

#endif /* !PRETTY_PRINT_H */
