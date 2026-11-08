#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "ast.h"

/*
 * Free brace group node
 * @param struct brace_group_node *node
 * @return void
 */

void brace_group_node_free(struct brace_group_node *node)
{
    if (!node)
        return;
    ast_release(node->command);
    free(node);
}
