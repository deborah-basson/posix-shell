/* Execution engine */

#define _POSIX_C_SOURCE 200809L
#include "exec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../builtins/builtins.h"
#include "../variables/variables.h"
#include "expansion/expansion.h"
#include "functions.h"
#include "global_state.h"
#include "pipeline.h"
#include "redirection.h"
#include "simple_command.h"

/* Execute function def */
static int exec_function_def(struct function_def_node *node)
{
    if (!node || !g_function_globals.table)
        return 0;

    struct func_def def = { node->body, node->redirs, node->redir_count };
    function_define(g_function_globals.table, node->name, &def);
    return 0;
}

/* Execute case */
static int exec_case(struct case_node *case_n)
{
    if (!case_n)
        return 0;

    char *expanded = expand_word(case_n->word);
    if (!expanded)
        expanded = strdup(case_n->word);

    int status = 0;

    struct case_item *item = case_n->items;
    while (item)
    {
        if (strcmp(expanded, item->pattern) == 0
            || strcmp(item->pattern, "*") == 0)
        {
            if (item->commands)
                status = exec_ast(item->commands);
            break;
        }
        item = item->next;
    }

    free(expanded);
    return status;
}

/* Execute list */
static int exec_list(struct list_node *list)
{
    int status = 0;
    for (int i = 0; i < list->count; i++)
    {
        status = exec_ast(list->commands[i]);
        g_shell.last_exit_status = status;
        fflush(stdout);

        if (g_builtin_state.break_level > 0
            || g_builtin_state.continue_flag > 0)
            break;
    }
    return status;
}

/* Execute if */
static int exec_if(struct if_node *node)
{
    int cond_status = exec_ast(node->condition);

    if (cond_status == 0)
        return exec_ast(node->then_part);

    for (int i = 0; i < node->elif_count; i++)
    {
        cond_status = exec_ast(node->elif_parts[i].condition);
        if (cond_status == 0)
            return exec_ast(node->elif_parts[i].then_part);
    }

    if (node->else_part)
        return exec_ast(node->else_part);

    return cond_status;
}

/* Execute redirection */
static int exec_redirection(struct command_with_redirs *cmd)
{
    struct saved_fd_pair *saved = NULL;

    if (setup_redirections(cmd->redirs, cmd->redir_count, &saved) == -1)
    {
        return 1;
    }

    int status = 0;
    if (cmd->command)
        status = exec_ast(cmd->command);

    fflush(stdout);
    fflush(stderr);

    restore_redirections(saved, cmd->redir_count);

    return status;
}

/* Execute negation */
static int exec_negation(struct negation_node *neg)
{
    int status = exec_ast(neg->command);
    return status == 0 ? 1 : 0;
}

/* Execute while */
static int exec_while(struct loop_node *loop)
{
    int status = 0;
    g_builtin_state.loop_depth++;

    while (1)
    {
        int cond = exec_ast(loop->condition);
        if (cond != 0)
            break;

        status = exec_ast(loop->body);

        if (g_builtin_state.break_level > 0)
        {
            g_builtin_state.break_level--;
            break;
        }

        if (g_builtin_state.continue_flag)
        {
            g_builtin_state.continue_flag = 0;
            continue;
        }
    }

    g_builtin_state.loop_depth--;
    return status;
}

/* Execute until */
static int exec_until(struct loop_node *loop)
{
    int status = 0;
    g_builtin_state.loop_depth++;

    while (1)
    {
        int cond = exec_ast(loop->condition);
        if (cond == 0)
            break;

        status = exec_ast(loop->body);

        if (g_builtin_state.break_level > 0)
        {
            g_builtin_state.break_level--;
            break;
        }

        if (g_builtin_state.continue_flag)
        {
            g_builtin_state.continue_flag = 0;
            continue;
        }
    }

    g_builtin_state.loop_depth--;
    return status;
}

/* Execute and or */
static int exec_and_or(struct and_or_node *node)
{
    int left_status = exec_ast(node->left);

    if (node->is_and)
    {
        if (left_status != 0)
            return left_status;
        /*
         * Execute AST
         * @param node->right
         * @return return
         */

        return exec_ast(node->right);
    }
    else
    {
        if (left_status == 0)
            return left_status;
        return exec_ast(node->right);
    }
}

/* Execute for */
static int exec_for(struct for_node *node)
{
    int status = 0;
    g_builtin_state.loop_depth++;

    for (int i = 0; i < node->word_count; i++)
    {
        var_set(g_var_table, node->variable, node->words[i]);
        status = exec_ast(node->body);

        if (g_builtin_state.break_level > 0)
        {
            g_builtin_state.break_level--;
            break;
        }

        if (g_builtin_state.continue_flag)
        {
            g_builtin_state.continue_flag = 0;
            continue;
        }
    }

    g_builtin_state.loop_depth--;
    return status;
}

/* Execute assignment */
static int exec_assignment(struct assignment_node *node)
{
    if (!node)
        return 0;

    char *expanded_value = expand_word(node->value);
    if (expanded_value)
    {
        var_set(g_var_table, node->name, expanded_value);
        free(expanded_value);
    }
    else
    {
        var_set(g_var_table, node->name, node->value);
    }

    return 0;
}

/* Execute command block */
static int exec_command_block(struct command_block *block)
{
    if (!block || !block->commands)
        return 0;

    /*
     * Execute AST
     * @param block->commands
     * @return return
     */

    return exec_ast(block->commands);
}

/* Execute subshell */
static int exec_subshell(struct subshell_node *subshell)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        int exit_code = exec_ast(subshell->command);
        exit(exit_code);
    }

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return 1;
}

/* Execute brace group */
static int exec_brace_group(struct brace_group_node *brace_group)
{
    /*
     * Execute AST
     * @param brace_group->command
     * @return return
     */

    return exec_ast(brace_group->command);
}

/* Execute ast basic */
static int exec_ast_basic(struct ast_node *node)
{
    switch (node->type)
    {
    case AST_SIMPLE_COMMAND:
        return exec_simple_command(node->data);
    case AST_LIST:
    case AST_COMPOUND_LIST:
        return exec_list(node->data);
    case AST_IF:
        return exec_if(node->data);
    case AST_REDIRECTION:
        return exec_redirection(node->data);
    case AST_PIPELINE:
        return exec_pipeline(node->data);
    case AST_NEGATION:
        return exec_negation(node->data);
    default:
        return -1;
    }
}

/* Execute ast control */
static int exec_ast_control(struct ast_node *node)
{
    switch (node->type)
    {
    case AST_WHILE:
        return exec_while(node->data);
    case AST_UNTIL:
        return exec_until(node->data);
    case AST_AND_OR:
        return exec_and_or(node->data);
    case AST_FOR:
        return exec_for(node->data);
    case AST_ASSIGNMENT:
        return exec_assignment(node->data);
    case AST_COMMAND_BLOCK:
        return exec_command_block(node->data);
    case AST_FUNCTION_DEF:
        return exec_function_def(node->data);
    case AST_SUBSHELL:
        return exec_subshell(node->data);
    case AST_BRACE_GROUP:
        return exec_brace_group(node->data);
    case AST_CASE:
        return exec_case(node->data);
    default:
        return 1;
    }
}

/*
 * Execute AST
 * @param struct ast_node *node
 * @return int
 */

int exec_ast(struct ast_node *node)
{
    if (!node)
        return 0;

    int status = exec_ast_basic(node);
    if (status != -1)
        return status;

    return exec_ast_control(node);
}
