#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

static struct token *create_redir(enum token_type type, const char *s, int len)
{
    char *value = strndup(s, len);
    struct token *tok = token_new(type, value);
    free(value);
    tok->io_number = -1;
    return tok;
}

/* Helper for match two char redir */
static int match_two_char_redir(const char *s, enum token_type *type)
{
    if (s[0] == '>' && s[1] == '>')
        return (*type = TOKEN_REDIR_APPEND, 2);
    if (s[0] == '>' && s[1] == '&')
        return (*type = TOKEN_REDIR_DUP_OUT, 2);
    if (s[0] == '<' && s[1] == '&')
        return (*type = TOKEN_REDIR_DUP_IN, 2);
    if (s[0] == '>' && s[1] == '|')
        return (*type = TOKEN_REDIR_CLOBBER, 2);
    if (s[0] == '<' && s[1] == '>')
        return (*type = TOKEN_REDIR_RDWR, 2);
    return 0;
}

/* Get redir type */
static int get_redir_type(const char *s, enum token_type *type)
{
    int len = match_two_char_redir(s, type);
    if (len > 0)
        return len;

    if (s[0] == '>')
        return (*type = TOKEN_REDIR_OUT, 1);
    if (s[0] == '<')
        return (*type = TOKEN_REDIR_IN, 1);

    return 0;
}

struct token *lex_redirection(struct lexer *lexer)
{
    const char *s = &lexer->input[lexer->pos];
    enum token_type type;
    int len = get_redir_type(s, &type);

    if (len == 0)
        return NULL;

    struct token *tok = create_redir(type, s, len);
    lexer->pos += len;
    return tok;
}
