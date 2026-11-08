#define _POSIX_C_SOURCE 200809L
#include "token.h"

#include <stdlib.h>
#include <string.h>

struct token *token_new(enum token_type type, const char *value)
{
    struct token *token = malloc(sizeof(struct token));
    if (!token)
        return NULL;
    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->io_number = -1;
    return token;
}

/*
 * Free token
 * @param struct token *token
 * @return void
 */

void token_free(struct token *token)
{
    if (!token)
        return;
    free(token->value);
    free(token);
}

/*
 * Handle token is redirection
 * @param enum token_type type
 * @return int
 */

int token_is_redirection(enum token_type type)
{
    return type >= TOKEN_REDIR_IN && type <= TOKEN_REDIR_RDWR;
}
