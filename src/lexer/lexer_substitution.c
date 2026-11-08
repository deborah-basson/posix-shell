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

char *handle_var_brace(struct lexer *lexer, char *word, size_t *len,
                       size_t *capacity)
{
    if (*len + 2 >= *capacity)
    {
        word = realloc_word(word, capacity);
        if (!word)
            return NULL;
    }
    word[(*len)++] = '$';
    word[(*len)++] = '{';
    lexer->pos += 2;

    while (lexer->pos < lexer->len && lexer->input[lexer->pos] != '}')
    {
        if (*len >= *capacity - 1)
        {
            word = realloc_word(word, capacity);
            if (!word)
                return NULL;
        }
        word[(*len)++] = lexer->input[lexer->pos];
        lexer->pos++;
    }

    if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '}')
    {
        if (*len >= *capacity - 1)
        {
            word = realloc_word(word, capacity);
            if (!word)
                return NULL;
        }
        word[(*len)++] = '}';
        lexer->pos++;
    }
    return word;
}
