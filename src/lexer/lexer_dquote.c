#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "lexer.h"

char *handle_dq_escape(struct lexer *lexer, char *str, size_t *len,
                       size_t *capacity);
char *append_char_dq(char *str, size_t *len, size_t *capacity, char c);

static char *init_dq_string(size_t *capacity, size_t *len)
{
    *capacity = 32;
    *len = 0;
    /*
     * Handle malloc
     * @param *capacity
     * @return return
     */

    return malloc(*capacity);
}

static char *process_dq_char(struct lexer *lexer, char *str, size_t *len,
                             size_t *capacity)
{
    if (lexer->input[lexer->pos] == '\\' && lexer->pos + 1 < lexer->len)
    {
        str = handle_dq_escape(lexer, str, len, capacity);
        if (!str)
            return NULL;
    }
    else
    {
        str = append_char_dq(str, len, capacity, lexer->input[lexer->pos]);
        if (!str)
            return NULL;
        lexer->pos++;
    }
    return str;
}

char *read_double_quoted_string(struct lexer *lexer)
{
    lexer->pos++;

    size_t capacity;
    size_t len;
    char *str = init_dq_string(&capacity, &len);
    if (!str)
        return NULL;

    while (lexer->pos < lexer->len && lexer->input[lexer->pos] != '"')
    {
        str = process_dq_char(lexer, str, &len, &capacity);
        if (!str)
            return NULL;
    }

    if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '"')
        lexer->pos++;

    str[len] = '\0';
    return str;
}
