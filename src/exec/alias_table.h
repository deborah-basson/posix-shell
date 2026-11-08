#ifndef ALIAS_TABLE_H
#define ALIAS_TABLE_H

#include <stddef.h>

#define ALIAS_TABLE_SIZE 128

struct alias_entry
{
    char *name;
    char *value;
    struct alias_entry *next;
};

struct alias_table
{
    struct alias_entry **buckets;
    size_t size;
};

struct alias_table *alias_table_create(void);
void alias_table_free(struct alias_table *table);

int alias_table_set(struct alias_table *table, const char *name,
                    const char *value);
const char *alias_table_get(struct alias_table *table, const char *name);
int alias_table_unset(struct alias_table *table, const char *name);

#endif /* !ALIAS_TABLE_H */
