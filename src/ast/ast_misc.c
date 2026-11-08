#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "ast.h"

struct assignment_node *assignment_node_new(const char *name, const char *value)
{
    struct assignment_node *node = malloc(sizeof(struct assignment_node));
    if (!node)
        return NULL;
    node->name = strdup(name);
    node->value = strdup(value);
    return node;
}

/*
 * Free assignment node
 * @param struct assignment_node *node
 * @return void
 */

void assignment_node_free(struct assignment_node *node)
{
    if (!node)
        return;
    free(node->name);
    free(node->value);
    free(node);
}

struct function_def_node *function_def_node_new(const char *name,
                                                struct ast_node *body)
{
    struct function_def_node *node = malloc(sizeof(struct function_def_node));
    if (!node)
        return NULL;
    node->name = strdup(name);
    node->body = body;
    node->redirs = NULL;
    node->redir_count = 0;
    return node;
}
