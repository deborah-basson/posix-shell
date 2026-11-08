#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "ast.h"

struct redirection *redirection_new(enum token_type type, int io_number,
                                    const char *target)
{
    struct redirection *redir = malloc(sizeof(struct redirection));
    if (!redir)
        return NULL;
    redir->redir_type = type;
    redir->io_number = io_number;
    redir->target = target ? strdup(target) : NULL;
    return redir;
}

/*
 * Free redirection
 * @param struct redirection *redir
 * @return void
 */

void redirection_free(struct redirection *redir)
{
    if (!redir)
        return;
    free(redir->target);
    free(redir);
}

struct command_with_redirs *command_with_redirs_new(void)
{
    struct command_with_redirs *cmd =
        malloc(sizeof(struct command_with_redirs));
    if (!cmd)
        return NULL;
    cmd->command = NULL;
    cmd->redirs = NULL;
    cmd->redir_count = 0;
    return cmd;
}
