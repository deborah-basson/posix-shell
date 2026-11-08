#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "ast.h"

/*
 * Free function def node
 * @param struct function_def_node *node
 * @return void
 */

void function_def_node_free(struct function_def_node *node)
{
    if (!node)
        return;
    free(node->name);
    ast_release(node->body);
    for (int i = 0; i < node->redir_count; i++)
        redirection_free(node->redirs[i]);
    free(node->redirs);
    free(node);
}

struct subshell_node *subshell_node_new(struct ast_node *command)
{
    struct subshell_node *node = malloc(sizeof(struct subshell_node));
    if (!node)
        return NULL;
    node->command = command;
    return node;
}

/*
 * Free subshell node
 * @param struct subshell_node *node
 * @return void
 */

void subshell_node_free(struct subshell_node *node)
{
    if (!node)
        return;
    ast_release(node->command);
    free(node);
}

struct brace_group_node *brace_group_node_new(struct ast_node *command)
{
    struct brace_group_node *node = malloc(sizeof(struct brace_group_node));
    if (!node)
        return NULL;
    node->command = command;
    return node;
}
