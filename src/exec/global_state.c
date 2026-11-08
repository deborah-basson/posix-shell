#include "global_state.h"

#include <stdlib.h>

struct shell_globals g_shell = { 0, 0, NULL };

/*
 * Handle init global state
 * @param void
 * @return void
 */

void init_global_state(void)
{
    if (!g_shell.alias_table)
        g_shell.alias_table = alias_table_create();
}

/*
 * Handle cleanup global state
 * @param void
 * @return void
 */

void cleanup_global_state(void)
{
    if (g_shell.alias_table)
    {
        alias_table_free(g_shell.alias_table);
        g_shell.alias_table = NULL;
    }
}
