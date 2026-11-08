#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#define SQUOTE_START '\x02'
#define SQUOTE_END '\x03'
#define DQUOTE_START '\x04'
#define DQUOTE_END '\x05'

int is_assignment(const char *word);
struct assignment_node *assignment_node_new(const char *name,
                                            const char *value);

/* Check if quote marker */
static int is_quote_marker(char c)
{
    return c == SQUOTE_START || c == SQUOTE_END || c == DQUOTE_START
        || c == DQUOTE_END;
}

static char *strip_quote_markers(const char *str)
{
    if (!str)
        return NULL;

    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (!result)
        return NULL;

    size_t j = 0;
    for (size_t i = 0; str[i]; i++)
    {
        if (!is_quote_marker(str[i]))
            result[j++] = str[i];
    }
    result[j] = '\0';
    return result;
}

struct ast_node *try_parse_assignment(struct token *tok)
{
    if (!is_assignment(tok->value))
        return NULL;

    char *value_copy = strdup(tok->value);
    char *eq = strchr(value_copy, '=');

    struct ast_node *assign_node = NULL;
    if (eq)
    {
        *eq = '\0';
        char *name = value_copy;
        char *raw_value = eq + 1;

        char *clean_value = strip_quote_markers(raw_value);
        if (!clean_value)
            clean_value = strdup("");

        struct assignment_node *assignment =
            assignment_node_new(name, clean_value);
        assign_node = ast_node_new(AST_ASSIGNMENT, assignment);
        free(clean_value);
    }
    free(value_copy);
    return assign_node;
}
