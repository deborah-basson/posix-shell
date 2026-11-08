#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H

#include "alias_table.h"

struct shell_globals
{
    int last_exit_status;
    int func_argc;
    struct alias_table *alias_table;
};

extern struct shell_globals g_shell;

void init_global_state(void);
void cleanup_global_state(void);

#endif /* !GLOBAL_STATE_H */
