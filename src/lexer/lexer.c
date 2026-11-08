/* Lexer - String tokenization */

#define _POSIX_C_SOURCE 200809L
#include "lexer.h"

#include <stdlib.h>
#include <string.h>

struct token *lex_token(struct lexer *lexer);

/*
 * Handle skip whitespace
 * @param struct lexer *lexer
 * @return void
 */

void skip_whitespace(struct lexer *lexer)
{
    while (lexer->pos < lexer->len
           && (lexer->input[lexer->pos] == ' '
               || lexer->input[lexer->pos] == '\t'))
        lexer->pos++;
}

/*
 * Handle skip comment
 * @param struct lexer *lexer
 * @return void
 */

void skip_comment(struct lexer *lexer)
{
    while (lexer->pos < lexer->len && lexer->input[lexer->pos] != '\n')
        lexer->pos++;
}

struct lexer *lexer_new(const char *input)
{
    struct lexer *lexer = malloc(sizeof(struct lexer));
    if (!lexer)
        return NULL;
    lexer->input = input;
    lexer->pos = 0;
    lexer->len = strlen(input);
    lexer->current = NULL;
    lexer->owns_input = 0;
    return lexer;
}

/*
 * Free lexer resources
 * @param struct lexer *lexer
 * @return void
 */

void lexer_free(struct lexer *lexer)
{
    if (!lexer)
        return;
    token_free(lexer->current);
    if (lexer->owns_input)
        free((char *)lexer->input);
    free(lexer);
}

struct token *lexer_peek(struct lexer *lexer)
{
    if (!lexer->current)
        lexer->current = lex_token(lexer);
    return lexer->current;
}

struct token *lexer_next(struct lexer *lexer)
{
    if (lexer->current)
    {
        token_free(lexer->current);
        lexer->current = NULL;
    }
    return lexer_peek(lexer);
}
