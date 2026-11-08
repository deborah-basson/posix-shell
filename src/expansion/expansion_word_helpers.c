#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "expansion.h"

char *ensure_capacity_expand(char *result, size_t *capacity, size_t needed)
{
    while (*capacity <= needed)
    {
        *capacity *= 2;
        char *new_result = realloc(result, *capacity);
        if (!new_result)
        {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    return result;
}

char *append_expansion(char *result, size_t *result_len, size_t *capacity,
                       char *expansion)
{
    if (!expansion)
        return result;

    size_t exp_len = strlen(expansion);
    result = ensure_capacity_expand(result, capacity, *result_len + exp_len);
    if (!result)
    {
        free(expansion);
        return NULL;
    }

    strcpy(result + *result_len, expansion);
    *result_len += exp_len;
    free(expansion);
    return result;
}

char *append_char_expand(char *result, size_t *result_len, size_t *capacity,
                         char c)
{
    result = ensure_capacity_expand(result, capacity, *result_len);
    if (!result)
        return NULL;

    result[(*result_len)++] = c;
    return result;
}
