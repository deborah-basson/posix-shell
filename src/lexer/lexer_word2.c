#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

#define SQUOTE_START '\x02'
#define SQUOTE_END '\x03'
#define DQUOTE_START '\x04'
#define DQUOTE_END '\x05'

char *handle_var_brace(struct lexer *lexer, char *word, size_t *len,
                       size_t *capacity);
char *handle_command_sub(struct lexer *lexer, char *word, size_t *len,
                         size_t *capacity);
char *handle_escape(struct lexer *lexer, char *word, size_t *len,
                    size_t *capacity);

/* Check if special char */
static int is_special_char(char c)
{
    return c == ';' || c == '\n' || c == '\0' || c == ' ' || c == '\t'
        || c == '<' || c == '>' || c == '|' || c == '&' || c == '!' || c == '{'
        || c == '}' || c == '(' || c == ')';
}

static char *realloc_word(char *word, size_t *capacity)
{
    *capacity *= 2;
    char *new_word = realloc(word, *capacity);
    if (!new_word)
        free(word);
    return new_word;
}

static char *ensure_capacity(char *word, size_t *len, size_t *capacity,
                             size_t needed)
{
    while (*len + needed >= *capacity)
    {
        word = realloc_word(word, capacity);
        if (!word)
            return NULL;
    }
    return word;
}

static char *handle_single_quote(struct lexer *lexer, char *word, size_t *len,
                                 size_t *capacity)
{
    lexer->pos++;
    size_t start = lexer->pos;

    while (lexer->pos < lexer->len && lexer->input[lexer->pos] != '\'')
        lexer->pos++;

    if (lexer->pos >= lexer->len)
    {
        free(word);
        return NULL;
    }

    size_t content_len = lexer->pos - start;
    word = ensure_capacity(word, len, capacity, content_len + 3);
    if (!word)
        return NULL;

    word[(*len)++] = SQUOTE_START;
    memcpy(word + *len, lexer->input + start, content_len);
    *len += content_len;
    word[(*len)++] = SQUOTE_END;
    lexer->pos++;
    return word;
}

static char *read_dquote_content(struct lexer *lexer, char *word, size_t *len,
                                 size_t *capacity);

static char *handle_double_quote(struct lexer *lexer, char *word, size_t *len,
                                 size_t *capacity)
{
    lexer->pos++;
    word = ensure_capacity(word, len, capacity, 2);
    if (!word)
        return NULL;
    word[(*len)++] = DQUOTE_START;

    word = read_dquote_content(lexer, word, len, capacity);
    if (!word)
        return NULL;

    word = ensure_capacity(word, len, capacity, 2);
    if (!word)
        return NULL;
    word[(*len)++] = DQUOTE_END;

    if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '"')
        lexer->pos++;
    return word;
}

static char *handle_dq_escape_inline(struct lexer *lexer, char *word,
                                     size_t *len, size_t *capacity)
{
    lexer->pos++;
    if (lexer->pos >= lexer->len)
        return word;

    char next = lexer->input[lexer->pos];
    if (next == '\n')
    {
        lexer->pos++;
        return word;
    }
    if (next == '$' || next == '"' || next == '\\')
    {
        word = ensure_capacity(word, len, capacity, 2);
        if (word)
            word[(*len)++] = next;
        lexer->pos++;
        return word;
    }

    word = ensure_capacity(word, len, capacity, 3);
    if (word)
    {
        word[(*len)++] = '\\';
        word[(*len)++] = next;
    }
    lexer->pos++;
    return word;
}

static char *handle_dq_dollar(struct lexer *lexer, char *word, size_t *len,
                              size_t *capacity)
{
    word = ensure_capacity(word, len, capacity, 2);
    if (word)
        word[(*len)++] = lexer->input[lexer->pos++];
    return word;
}

static char *read_dquote_content(struct lexer *lexer, char *word, size_t *len,
                                 size_t *capacity)
{
    while (lexer->pos < lexer->len && lexer->input[lexer->pos] != '"')
    {
        if (lexer->input[lexer->pos] == '\\')
        {
            word = handle_dq_escape_inline(lexer, word, len, capacity);
        }

        /*
         * Handle if
         * @param lexer->input[lexer->pos] == '$'
         * @return else
         */

        else if (lexer->input[lexer->pos] == '$')
        {
            word = handle_dq_dollar(lexer, word, len, capacity);
        }
        else
        {
            word = ensure_capacity(word, len, capacity, 2);
            if (word)
                word[(*len)++] = lexer->input[lexer->pos++];
        }
        if (!word)
            return NULL;
    }
    return word;
}

static char *process_word_char(struct lexer *lexer, char *word, size_t *len,
                               size_t *capacity)
{
    char c = lexer->input[lexer->pos];

    if (c == '\'')
        return handle_single_quote(lexer, word, len, capacity);
    if (c == '"')
        return handle_double_quote(lexer, word, len, capacity);
    if (c == '\\' && lexer->pos + 1 < lexer->len)
        /*
         * Handle handle escape
         * @param lexer, word, len, capacity
         * @return return
         */

        return handle_escape(lexer, word, len, capacity);
    if (c == '$' && lexer->pos + 1 < lexer->len
        && lexer->input[lexer->pos + 1] == '{')
        return handle_var_brace(lexer, word, len, capacity);
    if (c == '$' && lexer->pos + 1 < lexer->len
        && lexer->input[lexer->pos + 1] == '(')
        return handle_command_sub(lexer, word, len, capacity);

    if (*len >= *capacity - 1)
        word = realloc_word(word, capacity);
    if (word)
        word[(*len)++] = lexer->input[lexer->pos++];
    return word;
}

char *read_word(struct lexer *lexer)
{
    size_t capacity = 32;
    size_t len = 0;
    char *word = malloc(capacity);
    if (!word)
        return NULL;

    while (lexer->pos < lexer->len)
    {
        char c = lexer->input[lexer->pos];

        if (c == '\'')
        {
            lexer->pos++;
            while (lexer->pos < lexer->len && lexer->input[lexer->pos] != '\'')
            {
                if (len >= capacity - 1)
                    word = realloc_word(word, &capacity);
                if (!word)
                    return NULL;
                word[len++] = lexer->input[lexer->pos++];
            }
            if (lexer->pos < lexer->len)
                lexer->pos++;
            continue;
        }

        if (c == '"')
        {
            lexer->pos++;
            while (lexer->pos < lexer->len && lexer->input[lexer->pos] != '"')
            {
                if (lexer->input[lexer->pos] == '\\'
                    && lexer->pos + 1 < lexer->len)
                {
                    lexer->pos++;
                    if (len >= capacity - 1)
                        word = realloc_word(word, &capacity);
                    if (!word)
                        return NULL;
                    word[len++] = lexer->input[lexer->pos++];
                }
                else
                {
                    if (len >= capacity - 1)
                        word = realloc_word(word, &capacity);
                    if (!word)
                        return NULL;
                    word[len++] = lexer->input[lexer->pos++];
                }
            }
            if (lexer->pos < lexer->len)
                lexer->pos++;
            continue;
        }

        if (is_special_char(c))
            break;

        word = process_word_char(lexer, word, &len, &capacity);
        if (!word)
            return NULL;
    }

    word[len] = '\0';
    return word;
}
