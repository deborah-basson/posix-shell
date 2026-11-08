#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

#include "exec.h"
#include "func_state.h"
#include "functions.h"
#include "global_state.h"
#include "redirection.h"

struct saved_args
{
    char **argv;
    int argc;
    int in_function;
};

/* Helper for save positional args */
static struct saved_args save_positional_args(void)
{
    struct saved_args saved;
    saved.argv = g_func_state.argv;
    saved.argc = g_shell.func_argc;
    saved.in_function = g_func_state.in_function;
    return saved;
}

/* Set positional args */
static void set_positional_args(char **argv, int argc)
{
    g_func_state.argv = argv;
    g_shell.func_argc = argc;
    g_func_state.in_function = 1;
}

/* Helper for restore positional args */
static void restore_positional_args(struct saved_args saved)
{
    g_func_state.argv = saved.argv;
    g_shell.func_argc = saved.argc;
    g_func_state.in_function = saved.in_function;
}

/* Helper for setup func redirections */
static int setup_func_redirections(struct function *func,
                                   struct saved_fd_pair **saved_fds)
{
    if (func->redir_count == 0)
        return 0;
    return setup_redirections(func->redirs, func->redir_count, saved_fds);
}

/* Helper for cleanup func redirections */
static void cleanup_func_redirections(struct function *func,
                                      struct saved_fd_pair *saved_fds)
{
    if (func->redir_count > 0)
    {
        fflush(stdout);
        fflush(stderr);
        restore_redirections(saved_fds, func->redir_count);
    }
}

/*
 * Execute function call
 * @param struct function *func, char **argv, int argc
 * @return int
 */

int exec_function_call(struct function *func, char **argv, int argc)
{
    if (!func || !func->body)
        return 0;

    struct saved_args saved = save_positional_args();
    set_positional_args(argv, argc);

    struct saved_fd_pair *saved_fds = NULL;
    if (setup_func_redirections(func, &saved_fds) == -1)
        return (restore_positional_args(saved), 1);

    int status = exec_ast(func->body);
    cleanup_func_redirections(func, saved_fds);
    restore_positional_args(saved);
    return status;
}
