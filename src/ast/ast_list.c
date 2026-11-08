#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "ast.h"

struct list_node *list_node_new(void)
{
    struct list_node *list = malloc(sizeof(struct list_node));
    if (!list)
        return NULL;
    list->commands = NULL;
    list->count = 0;
    return list;
}

/*
 * Handle list node add
 * @param struct list_node *list, struct ast_node *node
 * @return void
 */

void list_node_add(struct list_node *list, struct ast_node *node)
{
    list->commands =
        realloc(list->commands, sizeof(struct ast_node *) * (list->count + 1));
    list->commands[list->count++] = node;
}

/*
 * Free list node
 * @param struct list_node *list
 * @return void
 */

void list_node_free(struct list_node *list)
{
    if (!list)
        return;
    for (int i = 0; i < list->count; i++)
        ast_release(list->commands[i]);
    free(list->commands);
    free(list);
}
