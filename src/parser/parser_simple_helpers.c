#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "parser.h"

struct redirection *parse_redirection(struct lexer *lexer);

/*
 * Handle is keyword in command position
 * @param enum token_type type, int word_count
 * @return int
 */

int is_keyword_in_command_position(enum token_type type, int word_count)
{
    if (word_count == 0)
        return 0;

    return type == TOKEN_DONE || type == TOKEN_DO || type == TOKEN_THEN
        || type == TOKEN_FI || type == TOKEN_ELIF || type == TOKEN_ELSE
        || type == TOKEN_IN || type == TOKEN_FOR || type == TOKEN_WHILE
        || type == TOKEN_UNTIL || type == TOKEN_IF;
}

struct redirection **add_redirection(struct redirection **redirs,
                                     int *redir_count,
                                     struct redirection *redir)
{
    redirs = realloc(redirs, sizeof(struct redirection *) * (*redir_count + 1));
    redirs[*redir_count] = redir;
    (*redir_count)++;
    return redirs;
}

char **add_word(char **words, int *count, const char *value)
{
    words = realloc(words, sizeof(char *) * (*count + 1));
    words[*count] = strdup(value);
    (*count)++;
    return words;
}
