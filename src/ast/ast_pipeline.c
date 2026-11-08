#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "ast.h"

struct pipeline_node *pipeline_node_new(void)
{
    struct pipeline_node *pipeline = malloc(sizeof(struct pipeline_node));
    if (!pipeline)
        return NULL;
    pipeline->commands = NULL;
    pipeline->count = 0;
    return pipeline;
}

/*
 * Handle pipeline node add
 * @param struct pipeline_node *pipeline, struct ast_node *cmd
 * @return void
 */

void pipeline_node_add(struct pipeline_node *pipeline, struct ast_node *cmd)
{
    pipeline->commands = realloc(
        pipeline->commands, sizeof(struct ast_node *) * (pipeline->count + 1));
    pipeline->commands[pipeline->count++] = cmd;
}

/*
 * Free pipeline node
 * @param struct pipeline_node *pipeline
 * @return void
 */

void pipeline_node_free(struct pipeline_node *pipeline)
{
    if (!pipeline)
        return;
    for (int i = 0; i < pipeline->count; i++)
        ast_release(pipeline->commands[i]);
    free(pipeline->commands);
    free(pipeline);
}
