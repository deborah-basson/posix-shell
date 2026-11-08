#define _POSIX_C_SOURCE 200809L
#include "lexer_alias.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../exec/alias_table.h"
#include "../exec/global_state.h"

/*
 * Handle lexer try expand alias
 * @param struct lexer *lexer, const char *word,                           char
 * **expanded_input, size_t *expanded_len
 * @return int
 */

int lexer_try_expand_alias(struct lexer *lexer, const char *word,
                           char **expanded_input, size_t *expanded_len)
{
    if (!g_shell.alias_table || !word)
        return 0;

    const char *alias_value = alias_table_get(g_shell.alias_table, word);
    if (!alias_value)
        return 0;

    size_t remaining_len = lexer->len - lexer->pos;
    size_t alias_len = strlen(alias_value);
    size_t new_len = alias_len + remaining_len;

    char *new_input = malloc(new_len + 1);
    if (!new_input)
        return 0;

    memcpy(new_input, alias_value, alias_len);

    if (remaining_len > 0)
        memcpy(new_input + alias_len, lexer->input + lexer->pos, remaining_len);

    new_input[new_len] = '\0';

    *expanded_input = new_input;
    *expanded_len = new_len;

    return 1;
}
