#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "ast.h"

struct loop_node *loop_node_new(struct ast_node *cond, struct ast_node *body)
{
    struct loop_node *loop = malloc(sizeof(struct loop_node));
    if (!loop)
        return NULL;
    loop->condition = cond;
    loop->body = body;
    return loop;
}

/*
 * Free loop node
 * @param struct loop_node *loop
 * @return void
 */

void loop_node_free(struct loop_node *loop)
{
    if (!loop)
        return;
    ast_release(loop->condition);
    ast_release(loop->body);
    free(loop);
}
