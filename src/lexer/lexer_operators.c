#define _POSIX_C_SOURCE 200809L
#include "lexer.h"

struct token *lex_semicolon(struct lexer *lexer)
{
    if (lexer->pos + 1 < lexer->len && lexer->input[lexer->pos + 1] == ';')
    {
        lexer->pos += 2;
        return token_new(TOKEN_DSEMI, ";;");
    }

    lexer->pos++;
    /*
     * Create token
     * @param TOKEN_SEMICOLON, ";"
     * @return return
     */

    return token_new(TOKEN_SEMICOLON, ";");
}

struct token *lex_newline(struct lexer *lexer)
{
    lexer->pos++;
    /*
     * Create token
     * @param TOKEN_NEWLINE, "\n"
     * @return return
     */

    return token_new(TOKEN_NEWLINE, "\n");
}

struct token *lex_brace(struct lexer *lexer, char c)
{
    lexer->pos++;
    if (c == '{')
        return token_new(TOKEN_LBRACE, "{");
    /*
     * Create token
     * @param TOKEN_RBRACE, "}"
     * @return return
     */

    return token_new(TOKEN_RBRACE, "}");
}

struct token *lex_pipe(struct lexer *lexer)
{
    if (lexer->pos + 1 < lexer->len && lexer->input[lexer->pos + 1] == '|')
    {
        lexer->pos += 2;
        return token_new(TOKEN_OR_IF, "||");
    }
    lexer->pos++;
    /*
     * Create token
     * @param TOKEN_PIPE, "|"
     * @return return
     */

    return token_new(TOKEN_PIPE, "|");
}

struct token *lex_ampersand(struct lexer *lexer)
{
    if (lexer->pos + 1 < lexer->len && lexer->input[lexer->pos + 1] == '&')
    {
        lexer->pos += 2;
        return token_new(TOKEN_AND_IF, "&&");
    }
    return NULL;
}

struct token *lex_bang(struct lexer *lexer)
{
    lexer->pos++;
    /*
     * Create token
     * @param TOKEN_BANG, "!"
     * @return return
     */

    return token_new(TOKEN_BANG, "!");
}

struct token *lex_paren(struct lexer *lexer, char c)
{
    lexer->pos++;
    if (c == '(')
        return token_new(TOKEN_LPAREN, "(");
    return token_new(TOKEN_RPAREN, ")");
}
