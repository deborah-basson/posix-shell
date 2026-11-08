#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "ast.h"

/* Free basic nodes */
static void free_basic_nodes(struct ast_node *node)
{
    switch (node->type)
    {
    case AST_SIMPLE_COMMAND:
        simple_command_free((struct simple_command *)node->data);
        break;
    case AST_LIST:
    case AST_COMPOUND_LIST:
        list_node_free((struct list_node *)node->data);
        break;
    case AST_IF:
        if_node_free((struct if_node *)node->data);
        break;
    case AST_REDIRECTION:
        command_with_redirs_free((struct command_with_redirs *)node->data);
        break;
    default:
        break;
    }
}

/* Free control flow nodes */
static void free_control_flow_nodes(struct ast_node *node)
{
    switch (node->type)
    {
    case AST_PIPELINE:
        pipeline_node_free((struct pipeline_node *)node->data);
        break;
    case AST_NEGATION:
        negation_node_free((struct negation_node *)node->data);
        break;
    case AST_WHILE:
    case AST_UNTIL:
        loop_node_free((struct loop_node *)node->data);
        break;
    case AST_AND_OR:
        and_or_node_free((struct and_or_node *)node->data);
        break;
    case AST_FOR:
        for_node_free((struct for_node *)node->data);
        break;
    default:
        break;
    }
}

/* Free case items */
static void free_case_items(struct case_item *item)
{
    while (item)
    {
        struct case_item *next = item->next;
        free(item->pattern);
        ast_free(item->commands);
        free(item);
        item = next;
    }
}

/* Free case node */
static void free_case_node(struct case_node *case_n)
{
    if (!case_n)
        return;
    free(case_n->word);
    free_case_items(case_n->items);
    free(case_n);
}

/* Free compound nodes */
static void free_compound_nodes(struct ast_node *node)
{
    switch (node->type)
    {
    case AST_ASSIGNMENT:
        assignment_node_free((struct assignment_node *)node->data);
        break;
    case AST_COMMAND_BLOCK:
        if (node->data)
        {
            ast_free(((struct command_block *)node->data)->commands);
            free(node->data);
        }
        break;
    case AST_FUNCTION_DEF:
        function_def_node_free((struct function_def_node *)node->data);
        break;
    case AST_SUBSHELL:
        subshell_node_free((struct subshell_node *)node->data);
        break;
    case AST_BRACE_GROUP:
        brace_group_node_free((struct brace_group_node *)node->data);
        break;
    case AST_CASE:
        free_case_node((struct case_node *)node->data);
        break;
    default:
        break;
    }
}

/*
 * Free AST recursively
 * @param struct ast_node *node
 * @return void
 */

void ast_free(struct ast_node *node)
{
    if (!node)
        return;
    free_basic_nodes(node);
    free_control_flow_nodes(node);
    free_compound_nodes(node);
    free(node);
}
