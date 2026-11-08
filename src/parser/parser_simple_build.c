#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "parser.h"

struct simple_command *simple_command_new(char **words, int count);
struct command_with_redirs *command_with_redirs_new(void);

/* Free words array */
static void free_words_array(char **words, int count)
{
    for (int i = 0; i < count; i++)
        free(words[i]);
    free(words);
}

struct ast_node *build_simple_node(char **words, int count)
{
    struct simple_command *cmd = simple_command_new(words, count);
    free_words_array(words, count);
    /*
     * Create AST node
     * @param AST_SIMPLE_COMMAND, cmd
     * @return return
     */

    return ast_node_new(AST_SIMPLE_COMMAND, cmd);
}

struct ast_node *build_redir_node(char **words, int count,
                                  struct redirection **redirs, int redir_count)
{
    struct command_with_redirs *cmd_redir = command_with_redirs_new();

    if (count > 0)
    {
        struct simple_command *cmd = simple_command_new(words, count);
        cmd_redir->command = ast_node_new(AST_SIMPLE_COMMAND, cmd);
    }

    free_words_array(words, count);
    cmd_redir->redirs = redirs;
    cmd_redir->redir_count = redir_count;
    return ast_node_new(AST_REDIRECTION, cmd_redir);
}
