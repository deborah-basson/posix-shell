#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exec.h"
#include "functions.h"
#include "global_state.h"
#include "redirection.h"

struct function_table *function_table_new(void)
{
    struct function_table *table = malloc(sizeof(struct function_table));
    if (!table)
        return NULL;
    table->functions = NULL;
    table->count = 0;
    table->capacity = 0;
    return table;
}

static struct redirection *redirection_copy(struct redirection *redir)
{
    if (!redir)
        return NULL;
    /*
     * Create redirection
     * @param redir->redir_type, redir->io_number, redir->target
     * @return return
     */

    return redirection_new(redir->redir_type, redir->io_number, redir->target);
}

/* Helper for function free */
static void function_free(struct function *func)
{
    if (!func)
        return;
    free(func->name);
    ast_release(func->body);
    for (int i = 0; i < func->redir_count; i++)
        redirection_free(func->redirs[i]);
    free(func->redirs);
    free(func);
}

/*
 * Free function table
 * @param struct function_table *table
 * @return void
 */

void function_table_free(struct function_table *table)
{
    if (!table)
        return;
    for (int i = 0; i < table->count; i++)
        function_free(table->functions[i]);
    free(table->functions);
    free(table);
}

/* Helper for find function index */
static int find_function_index(struct function_table *table, const char *name)
{
    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->functions[i]->name, name) == 0)
            return i;
    }
    return -1;
}

/* Helper for ensure capacity */
static void ensure_capacity(struct function_table *table)
{
    if (table->count >= table->capacity)
    {
        int new_cap = table->capacity == 0 ? 8 : table->capacity * 2;
        struct function **new_funcs =
            realloc(table->functions, sizeof(struct function *) * new_cap);
        if (new_funcs)
        {
            table->functions = new_funcs;
            table->capacity = new_cap;
        }
    }
}

/* Helper for copy redirections */
static void copy_redirections(struct redirection ***dest, int *dest_count,
                              struct redirection **src, int src_count)
{
    *dest = NULL;
    *dest_count = 0;
    if (src_count > 0 && src)
    {
        *dest = malloc(sizeof(struct redirection *) * src_count);
        for (int i = 0; i < src_count; i++)
            (*dest)[i] = redirection_copy(src[i]);
        *dest_count = src_count;
    }
}

/* Helper for update existing function */
static void update_existing_function(struct function *func,
                                     struct func_def *def)
{
    ast_release(func->body);
    for (int i = 0; i < func->redir_count; i++)
        redirection_free(func->redirs[i]);
    free(func->redirs);

    ast_retain(def->body);
    func->body = def->body;
    copy_redirections(&func->redirs, &func->redir_count, def->redirs,
                      def->redir_count);
}

static struct function *create_new_function(const char *name,
                                            struct func_def *def)
{
    struct function *func = malloc(sizeof(struct function));
    if (!func)
        return NULL;

    func->name = strdup(name);
    func->body = def->body;
    func->ref_count = 1;
    ast_retain(def->body);
    copy_redirections(&func->redirs, &func->redir_count, def->redirs,
                      def->redir_count);
    return func;
}

/*
 * Handle function define
 * @param struct function_table *table, const char *name, struct func_def *def
 * @return void
 */

void function_define(struct function_table *table, const char *name,
                     struct func_def *def)
{
    if (!table || !name || !def || !def->body)
        return;

    int idx = find_function_index(table, name);
    if (idx >= 0)
    {
        update_existing_function(table->functions[idx], def);
        return;
    }

    ensure_capacity(table);
    struct function *func = create_new_function(name, def);
    if (!func)
        return;

    table->functions[table->count++] = func;
}

struct function *function_get(struct function_table *table, const char *name)
{
    if (!table || !name)
        return NULL;

    int idx = find_function_index(table, name);
    return idx >= 0 ? table->functions[idx] : NULL;
}

/*
 * Handle function unset
 * @param struct function_table *table, const char *name
 * @return void
 */

void function_unset(struct function_table *table, const char *name)
{
    if (!table || !name)
        return;

    int idx = find_function_index(table, name);
    if (idx < 0)
        return;

    function_free(table->functions[idx]);

    for (int i = idx; i < table->count - 1; i++)
        table->functions[i] = table->functions[i + 1];

    table->count--;
}

struct saved_args
{
    char **argv;
    int argc;
    int in_function;
};
