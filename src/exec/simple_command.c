#define _POSIX_C_SOURCE 200809L
#include "simple_command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../builtins/builtins.h"
#include "../exec/alias_table.h"
#include "../exec/global_state.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "expansion/expansion.h"
#include "functions.h"

extern int exec_function_call(struct function *func, char **argv, int argc);
/*
 * Execute AST
 * @param struct ast_node *ast
 * @return extern int
 */

extern int exec_ast(struct ast_node *ast);

/* Helper for count argv */
static int count_argv(char **argv)
{
    int count = 0;
    while (argv[count])
        count++;
    return count;
}

/* Check builtin */
static int check_builtin(const char *name)
{
    return strcmp(name, "echo") == 0 || strcmp(name, "true") == 0
        || strcmp(name, "false") == 0 || strcmp(name, "break") == 0
        || strcmp(name, "continue") == 0 || strcmp(name, "alias") == 0
        || strcmp(name, "unalias") == 0;
}

/* Execute builtin command */
static int exec_builtin_command(char **argv, int argc)
{
    if (strcmp(argv[0], "echo") == 0)
        return builtin_echo(argv, argc);
    if (strcmp(argv[0], "true") == 0)
        return builtin_true(argv, argc);
    if (strcmp(argv[0], "false") == 0)
        return builtin_false(argv, argc);
    if (strcmp(argv[0], "break") == 0)
        return builtin_break(argv, argc);
    if (strcmp(argv[0], "continue") == 0)
        return builtin_continue(argv, argc);
    if (strcmp(argv[0], "alias") == 0)
        return builtin_alias(argv, argc);
    if (strcmp(argv[0], "unalias") == 0)
        return builtin_unalias(argv, argc);
    if (strcmp(argv[0], ":") == 0)
        return builtin_colon(argv, argc);
    return 127;
}

/* Execute external command */
static int exec_external_command(char **argv)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(argv[0], argv);
        fprintf(stderr, "%s: command not found\n", argv[0]);
        exit(127);
    }
    if (pid > 0)
    {
        int wstatus;
        waitpid(pid, &wstatus, 0);
        return WIFEXITED(wstatus) ? WEXITSTATUS(wstatus) : 1;
    }
    return 1;
}

/* Helper for dispatch command */
static int dispatch_command(char **argv, int argc)
{
    if (strcmp(argv[0], "exit") == 0)
        return builtin_exit(argv, argc);
    if (strcmp(argv[0], "cd") == 0)
        return builtin_cd(argv, argc);
    if (strcmp(argv[0], "export") == 0)
        return builtin_export(argv, argc);
    if (strcmp(argv[0], ".") == 0)
        return builtin_dot(argv, argc);
    if (strcmp(argv[0], "unset") == 0)
        return builtin_unset(argv, argc);
    return -1;
}

/* Helper for execute command */
static int execute_command(char **argv, int argc)
{
    struct function *func = function_get(g_function_globals.table, argv[0]);
    if (func)
        return exec_function_call(func, argv + 1, argc - 1);
    if (check_builtin(argv[0]))
        return exec_builtin_command(argv, argc);
    int status = dispatch_command(argv, argc);
    if (status == -1)
        status = exec_external_command(argv);
    return status;
}

static char *build_alias_command(const char *alias_value, char **argv, int argc)
{
    size_t cmd_len = strlen(alias_value);
    for (int i = 1; i < argc; i++)
        cmd_len += 1 + strlen(argv[i]);

    char *new_cmd = malloc(cmd_len + 1);
    if (!new_cmd)
        return NULL;

    strcpy(new_cmd, alias_value);
    size_t pos = strlen(alias_value);
    for (int i = 1; i < argc; i++)
    {
        new_cmd[pos++] = ' ';
        strcpy(new_cmd + pos, argv[i]);
        pos += strlen(argv[i]);
    }
    new_cmd[pos] = '\0';
    return new_cmd;
}

/* Execute alias expansion */
static int exec_alias_expansion(char *new_cmd)
{
    struct lexer *lexer = lexer_new(new_cmd);
    if (!lexer)
    {
        free(new_cmd);
        return 1;
    }
    struct ast_node *ast = parse(lexer);
    if (!ast)
    {
        lexer_free(lexer);
        free(new_cmd);
        return 2;
    }
    int status = exec_ast(ast);
    ast_free(ast);
    lexer_free(lexer);
    free(new_cmd);
    return status;
}

/*
 * Execute simple command
 * @param struct simple_command *cmd
 * @return int
 */

int exec_simple_command(struct simple_command *cmd)
{
    if (!cmd || cmd->argc == 0)
        return 0;

    if (g_shell.alias_table && cmd->argv[0])
    {
        const char *alias_value =
            alias_table_get(g_shell.alias_table, cmd->argv[0]);
        if (alias_value)
        {
            char *new_cmd =
                build_alias_command(alias_value, cmd->argv, cmd->argc);
            return new_cmd ? exec_alias_expansion(new_cmd) : 1;
        }
    }

    char **expanded_argv = expand_argv(cmd->argv, cmd->argc);
    if (!expanded_argv)
        return 1;

    int argc = count_argv(expanded_argv);
    if (argc == 0)
        return (free_expanded_argv(expanded_argv, 0), 0);

    int status = execute_command(expanded_argv, argc);
    return (free_expanded_argv(expanded_argv, 0), status);
}
