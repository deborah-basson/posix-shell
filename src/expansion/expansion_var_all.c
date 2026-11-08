#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "../exec/func_state.h"
#include "../exec/global_argc.h"
#include "../exec/global_state.h"
#include "../main_globals.h"
#include "expansion.h"

/* Get argv context */
static void get_argv_context(int *argc, char ***argv)
{
    if (g_func_state.in_function)
    {
        *argc = g_shell.func_argc;
        *argv = g_func_state.argv;
    }
    else
    {
        *argc = g_main.argc;
        *argv = g_main.argv;
    }
}

char *expand_all_args_var(void)
{
    int argc;
    char **argv;
    get_argv_context(&argc, &argv);

    if (argc == 0 || !argv)
        return strdup("");

    size_t total_len = 0;
    for (int i = 0; i < argc; i++)
        total_len += strlen(argv[i]) + 1;

    char *value = malloc(total_len + 1);
    if (!value)
        return strdup("");

    value[0] = '\0';
    for (int i = 0; i < argc; i++)
    {
        if (i > 0)
            strcat(value, " ");
        strcat(value, argv[i]);
    }
    return value;
}
