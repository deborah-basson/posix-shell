#ifndef VARIABLES_H
#define VARIABLES_H

struct var_table
{
    char **names;
    char **values;
    int count;
    int capacity;
};

struct var_table *var_table_new(void);
void var_table_free(struct var_table *table);
void var_set(struct var_table *table, const char *name, const char *value);
char *var_get(struct var_table *table, const char *name);
void var_unset(struct var_table *table, const char *name);

extern struct var_table *g_var_table;

void init_global_vars(void);
void cleanup_global_vars(void);

#endif /* !VARIABLES_H */
