#ifndef EXEC_H
#define EXEC_H

#include "../ast/ast.h"

extern int g_last_exit_status;
int exec_ast(struct ast_node *node);
struct exec_context;
int exec_node(struct ast_node *node, struct exec_context *ctx);
#endif /* !EXEC_H */
