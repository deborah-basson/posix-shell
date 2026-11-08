#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "lexer.h"

static char *realloc_word(char *word, size_t *capacity)
{
    *capacity *= 2;
    char *new_word = realloc(word, *capacity);
    if (!new_word)
        free(word);
    return new_word;
}

static char *ensure_capacity_and_add(char *word, size_t *len, size_t *capacity,
                                     char c)
{
    if (*len >= *capacity - 1)
    {
        word = realloc_word(word, capacity);
        if (!word)
            return NULL;
    }
    word[(*len)++] = c;
    return word;
}

char *handle_command_sub(struct lexer *lexer, char *word, size_t *len,
                         size_t *capacity)
{
    if (*len + 2 >= *capacity)
    {
        word = realloc_word(word, capacity);
        if (!word)
            return NULL;
    }
    word[(*len)++] = '$';
    word[(*len)++] = '(';
    lexer->pos += 2;

    int paren_depth = 1;
    while (lexer->pos < lexer->len && paren_depth > 0)
    {
        word = ensure_capacity_and_add(word, len, capacity,
                                       lexer->input[lexer->pos]);
        if (!word)
            return NULL;

        if (lexer->input[lexer->pos] == '(')
            paren_depth++;
        else if (lexer->input[lexer->pos] == ')')
            paren_depth--;

        lexer->pos++;
    }
    return word;
}

char *handle_escape(struct lexer *lexer, char *word, size_t *len,
                    size_t *capacity)
{
    lexer->pos++;
    if (*len >= *capacity - 1)
    {
        word = realloc_word(word, capacity);
        if (!word)
            return NULL;
    }
    word[(*len)++] = lexer->input[lexer->pos];
    lexer->pos++;
    return word;
}
