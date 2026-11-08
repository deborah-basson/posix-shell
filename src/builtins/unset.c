#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../exec/functions.h"
#include "../variables/variables.h"
#include "builtins.h"

struct unset_opts
{
    int *i;
    int *opt_func;
    int *opt_var;
};

/* Helper for function table remove */
static int function_table_remove(const char *name)
{
    if (!g_function_globals.table)
        return 0;
    struct function *func = function_get(g_function_globals.table, name);
    if (!func)
        return 0;
    function_unset(g_function_globals.table, name);
    return 1;
}

/* Helper for variable remove */
static int variable_remove(const char *name)
{
    extern struct var_table *g_var_table;
    if (!g_var_table)
        return 1;
    var_unset(g_var_table, name);
    return 1;
}

/* Parse unset options */
static int parse_unset_options(char **argv, int argc, struct unset_opts *opts)
{
    while (*opts->i < argc && argv[*opts->i][0] == '-')
    {
        if (strcmp(argv[*opts->i], "-f") == 0)
            *opts->opt_func = 1;
        else if (strcmp(argv[*opts->i], "-v") == 0)
            *opts->opt_var = 1;
        else if (strcmp(argv[*opts->i], "--") == 0)
            return (*opts->i)++, 0;
        else
            return (
                fprintf(stderr, "unset: invalid option: %s\n", argv[*opts->i]),
                2);
        (*opts->i)++;
    }
    return 0;
}

/*
 * Execute unset builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_unset(char **argv, int argc)
{
    if (argc < 2)
        return 0;

    int opt_func = 0;
    int opt_var = 0;
    int i = 1;

    struct unset_opts opts = { &i, &opt_func, &opt_var };

    int err = parse_unset_options(argv, argc, &opts);
    if (err)
        return err;

    if (!opt_func && !opt_var)
        opt_var = 1;

    int exit_status = 0;
    while (i < argc)
    {
        if (opt_func && !function_table_remove(argv[i]))
            exit_status = 1;
        if (opt_var)
            variable_remove(argv[i]), unsetenv(argv[i]);
        i++;
    }

    return exit_status;
}
