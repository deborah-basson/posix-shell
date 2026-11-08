#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "lexer.h"

char *ensure_capacity_quote(char *str, size_t *capacity, size_t len)
{
    if (len < *capacity - 1)
        return str;

    *capacity *= 2;
    char *new_str = realloc(str, *capacity);
    if (!new_str)
        free(str);
    return new_str;
}

char *read_until_quote(struct lexer *lexer, char *str, size_t *len,
                       size_t *capacity)
{
    while (lexer->pos < lexer->len && lexer->input[lexer->pos] != '\'')
    {
        str = ensure_capacity_quote(str, capacity, *len);
        if (!str)
            return NULL;
        str[(*len)++] = lexer->input[lexer->pos];
        lexer->pos++;
    }
    return str;
}

char *handle_escaped_quote(struct lexer *lexer, char *str, size_t *len,
                           size_t *capacity)
{
    str = ensure_capacity_quote(str, capacity, *len);
    if (!str)
        return NULL;

    str[(*len)++] = '\'';
    str[*len] = '\0';
    lexer->pos += 2;

    if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '\'')
    {
        lexer->pos++;
        while (lexer->pos < lexer->len && lexer->input[lexer->pos] != '\'')
        {
            str = ensure_capacity_quote(str, capacity, *len);
            if (!str)
                return NULL;
            str[(*len)++] = lexer->input[lexer->pos];
            lexer->pos++;
        }
        if (lexer->pos < lexer->len)
            lexer->pos++;
        str[*len] = '\0';
    }

    return str;
}
