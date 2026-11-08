#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "../ast/ast.h"

struct function
{
    char *name;
    struct ast_node *body;
    struct redirection **redirs;
    int redir_count;
    int ref_count;
};

struct function_table
{
    struct function **functions;
    int count;
    int capacity;
};

struct function_globals
{
    struct function_table *table;
};

struct func_def
{
    struct ast_node *body;
    struct redirection **redirs;
    int redir_count;
};

extern struct function_globals g_function_globals;

struct function_table *function_table_new(void);
void function_table_free(struct function_table *table);
void function_define(struct function_table *table, const char *name,
                     struct func_def *def);
struct function *function_get(struct function_table *table, const char *name);
void function_unset(struct function_table *table, const char *name);
int exec_function_call(struct function *func, char **argv, int argc);

#endif /* !FUNCTIONS_H */
