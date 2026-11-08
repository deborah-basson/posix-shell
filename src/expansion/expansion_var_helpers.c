#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../exec/func_state.h"
#include "../exec/global_argc.h"
#include "../exec/global_state.h"
#include "../main_globals.h"
#include "../variables/variables.h"
#include "expansion.h"

static char *create_int_string(int value, const char *fallback)
{
    char *str = malloc(16);
    if (str)
        snprintf(str, 16, "%d", value);
    else
        str = strdup(fallback);
    return str;
}

char *expand_special_var(const char *var_name)
{
    if (strcmp(var_name, "?") == 0)
        return create_int_string(g_shell.last_exit_status, "");
    if (strcmp(var_name, "$") == 0)
        return create_int_string(getpid(), "");
    if (strcmp(var_name, "RANDOM") == 0)
        return create_int_string(rand() % 32768, "0");
    if (strcmp(var_name, "UID") == 0)
        return create_int_string(getuid(), "0");
    return NULL;
}

/* Get argc value */
static int get_argc_value(void)
{
    if (g_func_state.in_function)
        return g_shell.func_argc;
    return g_main.argc;
}

char *expand_argc_var(void)
{
    return create_int_string(get_argc_value(), "0");
}
