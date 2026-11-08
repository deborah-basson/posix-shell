#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "ast.h"

/*
 * Handle command with redirs add redir
 * @param struct command_with_redirs *cmd, struct redirection *redir
 * @return void
 */

void command_with_redirs_add_redir(struct command_with_redirs *cmd,
                                   struct redirection *redir)
{
    cmd->redirs = realloc(
        cmd->redirs, sizeof(struct redirection *) * (cmd->redir_count + 1));
    cmd->redirs[cmd->redir_count++] = redir;
}

/*
 * Free command with redirs
 * @param struct command_with_redirs *cmd
 * @return void
 */

void command_with_redirs_free(struct command_with_redirs *cmd)
{
    if (!cmd)
        return;
    ast_release(cmd->command);
    for (int i = 0; i < cmd->redir_count; i++)
        redirection_free(cmd->redirs[i]);
    free(cmd->redirs);
    free(cmd);
}
