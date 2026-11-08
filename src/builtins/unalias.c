#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../exec/alias_table.h"
#include "../exec/global_state.h"
#include "builtins.h"

/*
 * Execute unalias builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_unalias(char **argv, int argc)
{
    if (argc < 2)
    {
        fprintf(stderr, "unalias: usage: unalias name [name ...]\n");
        return 2;
    }

    if (argc == 2 && strcmp(argv[1], "-a") == 0)
    {
        alias_table_free(g_shell.alias_table);
        g_shell.alias_table = alias_table_create();
        return 0;
    }

    int has_error = 0;

    for (int i = 1; i < argc; i++)
    {
        if (alias_table_unset(g_shell.alias_table, argv[i]) != 0)
        {
            fprintf(stderr, "unalias: %s: not found\n", argv[i]);
            has_error = 1;
        }
    }

    return has_error ? 1 : 0;
}
