#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "ast.h"

struct negation_node *negation_node_new(struct ast_node *cmd)
{
    struct negation_node *neg = malloc(sizeof(struct negation_node));
    if (!neg)
        return NULL;
    neg->command = cmd;
    return neg;
}

/*
 * Free negation node
 * @param struct negation_node *neg
 * @return void
 */

void negation_node_free(struct negation_node *neg)
{
    if (!neg)
        return;
    ast_release(neg->command);
    free(neg);
}
