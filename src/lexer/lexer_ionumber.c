#define _POSIX_C_SOURCE 200809L
#include <ctype.h>

#include "lexer.h"

struct token *lex_redirection(struct lexer *lexer);

struct token *lex_ionumber(struct lexer *lexer)
{
    size_t saved_pos = lexer->pos;
    int io_num = 0;
    while (lexer->pos < lexer->len && isdigit(lexer->input[lexer->pos]))
    {
        io_num = io_num * 10 + (lexer->input[lexer->pos] - '0');
        lexer->pos++;
    }

    if (lexer->pos < lexer->len
        && (lexer->input[lexer->pos] == '<' || lexer->input[lexer->pos] == '>'))
    {
        struct token *tok = lex_redirection(lexer);
        if (tok)
        {
            tok->io_number = io_num;
            return tok;
        }
    }

    lexer->pos = saved_pos;
    return NULL;
}
