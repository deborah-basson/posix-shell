#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "ast.h"

struct if_node *if_node_new(void)
{
    struct if_node *node = malloc(sizeof(struct if_node));
    if (!node)
        return NULL;
    node->condition = NULL;
    node->then_part = NULL;
    node->elif_parts = NULL;
    node->elif_count = 0;
    node->else_part = NULL;
    return node;
}

/*
 * Free if node
 * @param struct if_node *node
 * @return void
 */

void if_node_free(struct if_node *node)
{
    if (!node)
        return;
    ast_release(node->condition);
    ast_release(node->then_part);
    if (node->elif_parts)
    {
        for (int i = 0; i < node->elif_count; i++)
        {
            ast_release(node->elif_parts[i].condition);
            ast_release(node->elif_parts[i].then_part);
        }
        free(node->elif_parts);
    }
    ast_release(node->else_part);
    free(node);
}
