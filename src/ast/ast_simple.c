#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"
#include "ast.h"

struct simple_command *simple_command_new(char **argv, int argc)
{
    struct simple_command *cmd = malloc(sizeof(struct simple_command));
    if (!cmd)
        return NULL;
    cmd->argv = malloc(sizeof(char *) * (argc + 1));
    if (!cmd->argv)
    {
        free(cmd);
        return NULL;
    }
    for (int i = 0; i < argc; i++)
        cmd->argv[i] = str_dup(argv[i]);
    cmd->argv[argc] = NULL;
    cmd->argc = argc;
    return cmd;
}

/*
 * Free simple command
 * @param struct simple_command *cmd
 * @return void
 */

void simple_command_free(struct simple_command *cmd)
{
    if (!cmd)
        return;
    if (cmd->argv)
    {
        for (int i = 0; cmd->argv[i]; i++)
            free(cmd->argv[i]);
        free(cmd->argv);
    }
    free(cmd);
}
