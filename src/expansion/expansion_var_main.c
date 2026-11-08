/* Main entry point - Shell REPL */

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "../variables/variables.h"
#include "expansion.h"

char *parse_var_name(const char *word, size_t *pos, int *use_braces);
char *expand_special_var(const char *var_name);
char *expand_argc_var(void);
char *expand_all_args_var(void);
char *expand_positional_arg(const char *var_name);

static char *try_special_expansions(char *var_name)
{
    if (strcmp(var_name, "#") == 0)
        return (free(var_name), expand_argc_var());
    if (strcmp(var_name, "@") == 0 || strcmp(var_name, "*") == 0)
        return (free(var_name), expand_all_args_var());

    char *value = expand_positional_arg(var_name);
    if (value)
        return (free(var_name), value);
    return NULL;
}

char *expand_variable_at_pos(const char *word, size_t *pos)
{
    int use_braces;
    char *var_name = parse_var_name(word, pos, &use_braces);

    if (!var_name)
        return strdup("$");
    if (strlen(var_name) == 0)
        return (free(var_name), strdup(""));

    char *value = expand_special_var(var_name);
    if (value)
        return (free(var_name), value);

    value = try_special_expansions(var_name);
    if (value)
        return value;

    char *val = var_get(g_var_table, var_name);
    value = val ? strdup(val) : strdup("");
    free(var_name);
    return value ? value : strdup("");
}
