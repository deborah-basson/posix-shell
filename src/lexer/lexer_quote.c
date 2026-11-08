#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "lexer.h"

char *ensure_capacity_quote(char *str, size_t *capacity, size_t len);
char *read_until_quote(struct lexer *lexer, char *str, size_t *len,
                       size_t *capacity);
char *handle_escaped_quote(struct lexer *lexer, char *str, size_t *len,
                           size_t *capacity);

static char *init_quote_buffer(size_t *capacity, size_t *len)
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

char *read_single_quoted(struct lexer *lexer)
{
    lexer->pos++;

    size_t capacity;
    size_t len;
    char *str = init_quote_buffer(&capacity, &len);
    if (!str)
        return NULL;

    str = read_until_quote(lexer, str, &len, &capacity);
    if (!str || lexer->pos >= lexer->len)
        return (free(str), NULL);

    str[len] = '\0';
    lexer->pos++;

    if (lexer->pos < lexer->len - 1 && lexer->input[lexer->pos] == '\\'
        && lexer->input[lexer->pos + 1] == '\'')
        str = handle_escaped_quote(lexer, str, &len, &capacity);

    return str;
}
