#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../exec/alias_table.h"
#include "../exec/global_state.h"
#include "builtins.h"

/* Helper for validate alias name */
static int validate_alias_name(const char *name)
{
    if (!name[0] || (!isalpha(name[0]) && name[0] != '_'))
        return 0;
    for (const char *p = name; *p; p++)
    {
        if (!isalnum(*p) && *p != '_')
            return 0;
    }
    return 1;
}

/* Helper for remove quotes */
static void remove_quotes(char **value, size_t *len)
{
    if (*len >= 2)
    {
        if (((*value)[0] == '\'' && (*value)[*len - 1] == '\'')
            || ((*value)[0] == '"' && (*value)[*len - 1] == '"'))
        {
            (*value)[*len - 1] = '\0';
            (*value)++;
        }
    }
}

/* Helper for process alias arg */
static int process_alias_arg(char *arg, struct alias_table *table)
{
    char *equals = strchr(arg, '=');
    if (!equals)
        return (fprintf(stderr, "alias: usage: alias NAME=VALUE\n"), 2);
    *equals = '\0';
    char *name = arg;
    char *value = equals + 1;
    size_t value_len = strlen(value);
    remove_quotes(&value, &value_len);
    if (!validate_alias_name(name))
    {
        fprintf(stderr, "alias: invalid alias name: %s\n", name);
        return (*equals = '=', 1);
    }
    if (alias_table_set(table, name, value) != 0)
    {
        fprintf(stderr, "alias: failed to set alias\n");
        return (*equals = '=', 1);
    }
    return (*equals = '=', 0);
}

/*
 * Execute alias builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_alias(char **argv, int argc)
{
    if (!g_shell.alias_table)
    {
        fprintf(stderr, "ERROR: alias_table is NULL!\n");
        return 1;
    }
    if (argc < 2)
        return 0;

    for (int i = 1; i < argc; i++)
    {
        int ret = process_alias_arg(argv[i], g_shell.alias_table);
        if (ret != 0)
            return ret;
    }
    return 0;
}
