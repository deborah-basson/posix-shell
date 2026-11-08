/* AST - Node management */

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "../utils/utils.h"
#include "ast.h"

struct ast_node *ast_node_new(enum ast_node_type type, void *data)
{
    struct ast_node *node = xmalloc(sizeof(struct ast_node));
    if (!node)
        return NULL;
    node->type = type;
    node->data = data;
    node->ref_count = 1;
    return node;
}

/*
 * Increment ref count
 * @param struct ast_node *node
 * @return void
 */

void ast_retain(struct ast_node *node)
{
    if (node)
        node->ref_count++;
}

/*
 * Decrement ref count
 * @param struct ast_node *node
 * @return void
 */

void ast_release(struct ast_node *node)
{
    if (!node)
        return;

    node->ref_count--;
    if (node->ref_count <= 0)
        ast_free(node);
}
