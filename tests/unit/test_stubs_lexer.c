#include "ast/ast.h"
#include "builtins/builtins.h"
#include "exec/func_state.h"
#include "exec/functions.h"
#include "exec/global_state.h"
#include "main_globals.h"
#include "parser/parser.h"

struct main_globals g_main = { 0, NULL, 0 };
struct shell_globals g_shell = { 0, 0, NULL };
struct func_state g_func_state = { NULL, 0 };
struct function_globals g_function_globals = { NULL };
struct builtin_state g_builtin_state = { 0, 0, 0, 0, 0 };

int builtin_echo(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
int builtin_true(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
int builtin_false(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 1;
}
int builtin_break(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
int builtin_continue(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
int builtin_exit(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
int builtin_cd(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
int builtin_export(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
int builtin_dot(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
int builtin_exec(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
int builtin_unset(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
int builtin_colon(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
int builtin_alias(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
int builtin_unalias(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}

struct ast_node *parse(struct lexer *lexer)
{
    (void)lexer;
    return NULL;
}
void ast_free(struct ast_node *node)
{
    (void)node;
}
void ast_retain(struct ast_node *node)
{
    (void)node;
}
void ast_release(struct ast_node *node)
{
    (void)node;
}
struct redirection *redirection_new(enum token_type type, int io_number,
                                    const char *target)
{
    (void)type;
    (void)io_number;
    (void)target;
    return NULL;
}
void redirection_free(struct redirection *redir)
{
    (void)redir;
}
