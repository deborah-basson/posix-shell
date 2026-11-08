/* Builtin dispatcher */

#include "builtins.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct builtin_state g_builtin_state = { 0, 0, 0, 0, 0 };

/* Helper for dispatch builtin group1 */
static int dispatch_builtin_group1(char **argv, int argc)
{
    if (strcmp(argv[0], "true") == 0)
        return builtin_true(argv, argc);
    if (strcmp(argv[0], "false") == 0)
        return builtin_false(argv, argc);
    if (strcmp(argv[0], "echo") == 0)
        return builtin_echo(argv, argc);
    if (strcmp(argv[0], "exit") == 0)
        return builtin_exit(argv, argc);
    if (strcmp(argv[0], "cd") == 0)
        return builtin_cd(argv, argc);
    return -1;
}

/* Helper for dispatch builtin group2 */
static int dispatch_builtin_group2(char **argv, int argc)
{
    if (strcmp(argv[0], "export") == 0)
        return builtin_export(argv, argc);
    if (strcmp(argv[0], ".") == 0)
        return builtin_dot(argv, argc);
    if (strcmp(argv[0], ":") == 0)
        return builtin_colon(argv, argc);
    if (strcmp(argv[0], "break") == 0)
        return builtin_break(argv, argc);
    if (strcmp(argv[0], "continue") == 0)
        return builtin_continue(argv, argc);
    return -1;
}

/*
 * Execute builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_exec(char **argv, int argc)
{
    if (argc == 0)
        return -1;

    int ret = dispatch_builtin_group1(argv, argc);
    if (ret != -1)
        return ret;

    ret = dispatch_builtin_group2(argv, argc);
    if (ret != -1)
        return ret;

    if (strcmp(argv[0], "unset") == 0)
        return builtin_unset(argv, argc);
    if (strcmp(argv[0], "alias") == 0)
        return builtin_alias(argv, argc);
    if (strcmp(argv[0], "unalias") == 0)
        return builtin_unalias(argv, argc);

    return -1;
}

/* Parse loop count */
static int parse_loop_count(char **argv, int argc, const char *cmd_name)
{
    if (argc == 1)
        return 1;

    char *endptr;
    long n = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || n <= 0)
    {
        fprintf(stderr, "%s: %s: numeric argument required\n", cmd_name,
                argv[1]);
        return -1;
    }
    return (int)n;
}

/* Check in loop */
static int check_in_loop(const char *cmd_name)
{
    if (g_builtin_state.loop_depth == 0)
    {
        fprintf(stderr,
                "%s: only meaningful in a `for', `while', or `until' loop\n",
                cmd_name);
        return 1;
    }
    return 0;
}

/*
 * Execute break builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_break(char **argv, int argc)
{
    int n = parse_loop_count(argv, argc, "break");
    if (n == -1)
        return 2;

    if (check_in_loop("break"))
        return 1;

    if (n > g_builtin_state.loop_depth)
        n = g_builtin_state.loop_depth;

    g_builtin_state.break_level = n;
    return 0;
}

/*
 * Execute continue builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_continue(char **argv, int argc)
{
    int n = parse_loop_count(argv, argc, "continue");
    if (n == -1)
        return 2;

    if (check_in_loop("continue"))
        return 1;

    if (n > g_builtin_state.loop_depth)
        n = g_builtin_state.loop_depth;

    g_builtin_state.continue_flag = n;
    return 0;
}

/*
 * Execute colon builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_colon(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}
