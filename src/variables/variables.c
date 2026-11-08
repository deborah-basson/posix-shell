/* Variable management */

#define _POSIX_C_SOURCE 200809L
#include "variables.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../exec/functions.h"

struct var_table *g_var_table = NULL;

/*
 * Handle init global vars
 * @param void
 * @return void
 */

void init_global_vars(void)
{
    if (!g_var_table)
        g_var_table = var_table_new();
    if (!g_function_globals.table)
        g_function_globals.table = function_table_new();
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)))
    {
        var_set(g_var_table, "PWD", cwd);
        var_set(g_var_table, "OLDPWD", cwd);
    }
    var_set(g_var_table, "IFS", " \t\n");
}

/*
 * Handle cleanup global vars
 * @param void
 * @return void
 */

void cleanup_global_vars(void)
{
    if (g_var_table)
    {
        var_table_free(g_var_table);
        g_var_table = NULL;
    }
    if (g_function_globals.table)
    {
        function_table_free(g_function_globals.table);
        g_function_globals.table = NULL;
    }
}

struct var_table *var_table_new(void)
{
    struct var_table *table = malloc(sizeof(struct var_table));
    if (!table)
        return NULL;
    table->names = NULL;
    table->values = NULL;
    table->count = 0;
    table->capacity = 0;
    return table;
}

/*
 * Free var table
 * @param struct var_table *table
 * @return void
 */

void var_table_free(struct var_table *table)
{
    if (!table)
        return;
    for (int i = 0; i < table->count; i++)
    {
        free(table->names[i]);
        free(table->values[i]);
    }
    free(table->names);
    free(table->values);
    free(table);
}

/* Helper for ensure capacity */
static int ensure_capacity(struct var_table *table)
{
    if (table->count < table->capacity)
        return 1;
    int new_cap = table->capacity == 0 ? 16 : table->capacity * 2;
    char **new_names = realloc(table->names, sizeof(char *) * new_cap);
    char **new_values = realloc(table->values, sizeof(char *) * new_cap);
    if (!new_names || !new_values)
        return 0;
    table->names = new_names;
    table->values = new_values;
    table->capacity = new_cap;
    return 1;
}

/*
 * Handle var set
 * @param struct var_table *table, const char *name, const char *value
 * @return void
 */

void var_set(struct var_table *table, const char *name, const char *value)
{
    if (!table || !name)
        return;
    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->names[i], name) == 0)
        {
            free(table->values[i]);
            table->values[i] = strdup(value ? value : "");
            return;
        }
    }
    if (!ensure_capacity(table))
        return;
    table->names[table->count] = strdup(name);
    table->values[table->count] = strdup(value ? value : "");
    table->count++;
}

char *var_get(struct var_table *table, const char *name)
{
    if (!table || !name)
        return NULL;
    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->names[i], name) == 0)
            return table->values[i];
    }
    char *env_val = getenv(name);
    if (env_val)
        return env_val;
    return NULL;
}

/*
 * Handle var unset
 * @param struct var_table *table, const char *name
 * @return void
 */

void var_unset(struct var_table *table, const char *name)
{
    if (!table || !name)
        return;
    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->names[i], name) == 0)
        {
            free(table->names[i]);
            free(table->values[i]);
            for (int j = i; j < table->count - 1; j++)
            {
                table->names[j] = table->names[j + 1];
                table->values[j] = table->values[j + 1];
            }
            table->count--;
            return;
        }
    }
}
