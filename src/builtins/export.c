#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../variables/variables.h"
#include "builtins.h"

/* Helper for export with assignment */
static void export_with_assignment(char *arg)
{
    char *eq = strchr(arg, '=');
    size_t name_len = eq - arg;
    char *name = strndup(arg, name_len);
    char *value = eq + 1;

    if (name)
    {
        var_set(g_var_table, name, value);
        setenv(name, value, 1);
        free(name);
    }
}

/* Helper for export existing var */
static void export_existing_var(char *arg)
{
    char *value = var_get(g_var_table, arg);
    if (value)
        setenv(arg, value, 1);
}

/*
 * Execute export builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_export(char **argv, int argc)
{
    if (argc == 1)
        return 0;

    for (int i = 1; i < argc; i++)
    {
        if (strchr(argv[i], '='))
            export_with_assignment(argv[i]);
        else
            export_existing_var(argv[i]);
    }

    return 0;
}
