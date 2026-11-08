#define _POSIX_C_SOURCE 200809L
#include "field_splitting.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../variables/variables.h"

struct field_builder
{
    char **fields;
    int count;
    int capacity;
};

/* Check if character is in IFS string */
static int is_ifs_char(char c, const char *ifs)
{
    for (size_t i = 0; ifs[i] != '\0'; i++)
    {
        if (c == ifs[i])
            return 1;
    }
    return 0;
}

/*
 * Check if char is IFS whitespace
 * @param char c, const char *ifs
 * @return int
 */
int is_ifs_whitespace(char c, const char *ifs)
{
    return is_ifs_char(c, ifs) && (c == ' ' || c == '\t' || c == '\n');
}

/* Check if character is IFS non-whitespace */
static int is_ifs_non_whitespace(char c, const char *ifs)
{
    return is_ifs_char(c, ifs) && !is_ifs_whitespace(c, ifs);
}

/* Expand field builder capacity */
static void grow_builder(struct field_builder *builder)
{
    builder->capacity *= 2;
    char **new_fields =
        realloc(builder->fields, sizeof(char *) * builder->capacity);
    if (!new_fields)
    {
        free(builder->fields);
        builder->fields = NULL;
    }
    else
        builder->fields = new_fields;
}

/* Add field to builder */
static void add_field(struct field_builder *builder, const char *str,
                      size_t len)
{
    if (builder->count >= builder->capacity - 1)
    {
        grow_builder(builder);
        if (!builder->fields)
            return;
    }

    char *field = malloc(len + 1);
    if (field)
    {
        memcpy(field, str, len);
        field[len] = '\0';
        builder->fields[builder->count++] = field;
    }
}

/* Skip leading whitespace */
static void skip_leading_ws(const char *input, size_t *i, const char *ifs)
{
    while (input[*i] && is_ifs_whitespace(input[*i], ifs))
        (*i)++;
}

/* Process field delimiter */
static void handle_delimiter(const char *input, size_t *i, const char *ifs,
                             struct field_builder *builder)
{
    if (is_ifs_non_whitespace(input[*i], ifs))
    {
        (*i)++;
        if (!input[*i] || is_ifs_whitespace(input[*i], ifs))
            add_field(builder, "", 0);
    }
    skip_leading_ws(input, i, ifs);
}

static char **posix_split(const char *input, const char *ifs)
{
    struct field_builder builder = { NULL, 0, 16 };
    builder.fields = malloc(sizeof(char *) * builder.capacity);
    if (!builder.fields)
        return NULL;

    size_t i = 0;
    skip_leading_ws(input, &i, ifs);

    while (input[i])
    {
        size_t start = i;
        while (input[i] && !is_ifs_char(input[i], ifs))
            i++;
        add_field(&builder, input + start, i - start);
        if (!builder.fields || !input[i])
            break;
        handle_delimiter(input, &i, ifs, &builder);
    }

    if (builder.fields)
        builder.fields[builder.count] = NULL;
    return builder.fields;
}

/*
 * Get IFS variable
 * @param void
 * @return char *
 */
char *get_ifs(void)
{
    char *ifs_val = NULL;
    if (g_var_table)
        ifs_val = var_get(g_var_table, "IFS");

    if (!ifs_val)
        return strdup(" \t\n");
    return strdup(ifs_val);
}

/*
 * Split string into fields
 * @param const char *input, const char *ifs
 * @return char **
 */
char **field_split(const char *input, const char *ifs)
{
    if (!input)
        return NULL;

    if (!ifs || *ifs == '\0')
        return posix_split(input, " \t\n");

    return posix_split(input, ifs);
}

/*
 * Free split result
 * @param char **result
 * @return void
 */
void free_split_result(char **result)
{
    if (!result)
        return;

    for (int i = 0; result[i] != NULL; i++)
        free(result[i]);
    free(result);
}
