/* Main entry point - Shell REPL */

#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "lexer_alias.h"

void skip_whitespace(struct lexer *lexer);
void skip_comment(struct lexer *lexer);
struct token *lex_semicolon(struct lexer *lexer);
struct token *lex_newline(struct lexer *lexer);
struct token *lex_brace(struct lexer *lexer, char c);
struct token *lex_pipe(struct lexer *lexer);
struct token *lex_ampersand(struct lexer *lexer);
struct token *lex_bang(struct lexer *lexer);
struct token *lex_paren(struct lexer *lexer, char c);
struct token *lex_ionumber(struct lexer *lexer);
struct token *lex_redirection(struct lexer *lexer);
char *read_word(struct lexer *lexer);
/*
 * Get keyword type
 * @param const char *word
 * @return enum token_type
 */

enum token_type get_keyword_type(const char *word);

static struct token *lex_word_token(struct lexer *lexer)
{
    char *word = read_word(lexer);
    if (!word)
        /*
         * Create token
         * @param TOKEN_ERROR, "Memory error"
         * @return return
         */

        return token_new(TOKEN_ERROR, "Memory error");
    if (word[0] == '\0')
    {
        free(word);
        return token_new(TOKEN_ERROR, "Empty word");
    }
    enum token_type type = get_keyword_type(word);

    struct token *token = token_new(type, word);
    free(word);
    return token;
}

static struct token *lex_simple_chars(struct lexer *lexer, char c)
{
    if (c == ';')
        return lex_semicolon(lexer);
    if (c == '\n')
        /*
         * Lex newline
         * @param lexer
         * @return return
         */

        return lex_newline(lexer);
    if (c == '{' || c == '}')
        return lex_brace(lexer, c);
    if (c == '|')
        return lex_pipe(lexer);
    if (c == '&')
        return lex_ampersand(lexer);
    if (c == '!')
        return lex_bang(lexer);
    if (c == '(' || c == ')')
        return lex_paren(lexer, c);
    return NULL;
}

struct token *lex_token(struct lexer *lexer)
{
    skip_whitespace(lexer);
    if (lexer->pos >= lexer->len)
        return token_new(TOKEN_EOF, NULL);

    char c = lexer->input[lexer->pos];
    if (c == '#')
        return (skip_comment(lexer), lex_token(lexer));

    struct token *tok = lex_simple_chars(lexer, c);
    if (tok)
        return tok;

    if (isdigit(c) && (tok = lex_ionumber(lexer)))
        return tok;
    if ((c == '<' || c == '>') && (tok = lex_redirection(lexer)))
        return tok;

    return lex_word_token(lexer);
}
