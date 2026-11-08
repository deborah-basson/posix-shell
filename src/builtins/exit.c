#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../exec/exec.h"
#include "../exec/global_state.h"
#include "../variables/variables.h"
#include "builtins.h"

/*
 * Execute exit builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_exit(char **argv, int argc)
{
    int exit_code = g_shell.last_exit_status;

    if (argc > 1)
    {
        char *arg = argv[1];
        char *endptr;
        long val = strtol(arg, &endptr, 10);

        if (*endptr != '\0' || *arg == '\0')
        {
            fprintf(stderr, "posix-shell: exit: %s: numeric argument required\n", arg);
            exit_code = 2;
        }
        else
        {
            exit_code = (int)(val & 0xFF);
        }
    }

    g_builtin_state.should_exit = 1;
    g_builtin_state.exit_code = exit_code;

    return exit_code;
}
