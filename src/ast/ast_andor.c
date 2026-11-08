#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "ast.h"

struct and_or_node *and_or_node_new(struct ast_node *left,
                                    struct ast_node *right, int is_and)
{
    struct and_or_node *node = malloc(sizeof(struct and_or_node));
    if (!node)
        return NULL;
    node->left = left;
    node->right = right;
    node->is_and = is_and;
    return node;
}

/*
 * Free and or node
 * @param struct and_or_node *node
 * @return void
 */

void and_or_node_free(struct and_or_node *node)
{
    if (!node)
        return;
    ast_release(node->left);
    ast_release(node->right);
    free(node);
}
