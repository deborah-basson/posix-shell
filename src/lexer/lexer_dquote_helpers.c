#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "lexer.h"

struct dq_buffer
{
    char *str;
    size_t *len;
    size_t *capacity;
};

static char *realloc_str(char *str, size_t *capacity)
{
    *capacity *= 2;
    char *new_str = realloc(str, *capacity);
    if (!new_str)
        free(str);
    return new_str;
}

char *append_char_dq(char *str, size_t *len, size_t *capacity, char c)
{
    if (*len >= *capacity - 1)
    {
        str = realloc_str(str, capacity);
        if (!str)
            return NULL;
    }
    str[(*len)++] = c;
    return str;
}

static char *handle_dollar_escape(struct dq_buffer *buf)
{
    if (*buf->len >= *buf->capacity - 3)
    {
        buf->str = realloc_str(buf->str, buf->capacity);
        if (!buf->str)
            return NULL;
    }
    buf->str[(*buf->len)++] = '\x01';
    buf->str[(*buf->len)++] = '$';
    return buf->str;
}

static char *handle_special_escape(struct lexer *lexer, struct dq_buffer *buf,
                                   char next)
{
    if (next == '\n')
        return (lexer->pos++, buf->str);

    if (next == '$')
        return (buf->str = handle_dollar_escape(buf), lexer->pos++, buf->str);

    return (buf->str = append_char_dq(buf->str, buf->len, buf->capacity, next),
            lexer->pos++, buf->str);
}

static char *handle_unknown_escape(struct dq_buffer *buf, char next)
{
    if (*buf->len >= *buf->capacity - 2)
    {
        buf->str = realloc_str(buf->str, buf->capacity);
        if (!buf->str)
            return NULL;
    }
    buf->str[(*buf->len)++] = '\\';
    buf->str[(*buf->len)++] = next;
    return buf->str;
}

char *handle_dq_escape(struct lexer *lexer, char *str, size_t *len,
                       size_t *capacity)
{
    lexer->pos++;
    char next = lexer->input[lexer->pos];

    struct dq_buffer buf = { str, len, capacity };

    if (next == '$' || next == '"' || next == '\\' || next == '\n')
        return handle_special_escape(lexer, &buf, next);

    str = handle_unknown_escape(&buf, next);
    lexer->pos++;
    return str;
}
