#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../exec/func_state.h"
#include "../exec/global_argc.h"
#include "../exec/global_state.h"
#include "../main_globals.h"
#include "expansion.h"

char *expand_positional_arg(const char *var_name)
{
    if (strlen(var_name) != 1 || !isdigit(var_name[0]))
        return NULL;

    int arg_num = var_name[0] - '0';

    if (arg_num == 0)
        /*
         * Handle strdup
         * @param "posix-shell"
         * @return return
         */

        return strdup("posix-shell");

    if (g_func_state.in_function)
    {
        if (g_shell.func_argc > 0 && arg_num <= g_shell.func_argc)
            return strdup(g_func_state.argv[arg_num - 1]);
        return strdup("");
    }

    if (g_main.argc > 0 && arg_num <= g_main.argc)
        return strdup(g_main.argv[arg_num - 1]);
    return strdup("");
}
