#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "expansion.h"

/* Check if special char */
static int is_special_char(char c)
{
    return c == '?' || c == '$' || c == '#' || c == '@' || c == '*';
}

/* Parse var name content */
static void parse_var_name_content(const char *word, size_t *pos,
                                   int use_braces)
{
    if (is_special_char(word[*pos]) || isdigit(word[*pos]))
    {
        (*pos)++;
        return;
    }

    if (isalpha(word[*pos]) || word[*pos] == '_')
    {
        while (word[*pos] != '\0' && (isalnum(word[*pos]) || word[*pos] == '_'))
        {
            if (use_braces && word[*pos] == '}')
                break;
            (*pos)++;
        }
    }
}

char *parse_var_name(const char *word, size_t *pos, int *use_braces)
{
    size_t start_pos = *pos;
    (*pos)++;

    if (word[*pos] == '\0')
        return (*pos = start_pos + 1, NULL);

    *use_braces = (word[*pos] == '{');
    if (*use_braces)
        (*pos)++;

    size_t var_start = *pos;
    parse_var_name_content(word, pos, *use_braces);

    size_t len = *pos - var_start;
    if (len == 0 && !isalpha(word[var_start]) && word[var_start] != '_')
        return (*pos = start_pos + 1, NULL);

    if (*use_braces && word[*pos] == '}')
        (*pos)++;

    return len == 0 ? strdup("") : strndup(&word[var_start], len);
}
