#define _POSIX_C_SOURCE 200809L
#include "alias_table.h"

#include <stdlib.h>
#include <string.h>

/* Helper for hash string */
static size_t hash_string(const char *str)
{
    size_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash % ALIAS_TABLE_SIZE;
}

struct alias_table *alias_table_create(void)
{
    struct alias_table *table = malloc(sizeof(struct alias_table));
    if (!table)
        return NULL;

    table->size = ALIAS_TABLE_SIZE;
    table->buckets = calloc(table->size, sizeof(struct alias_entry *));
    if (!table->buckets)
    {
        free(table);
        return NULL;
    }

    return table;
}

/*
 * Free alias table
 * @param struct alias_table *table
 * @return void
 */

void alias_table_free(struct alias_table *table)
{
    if (!table)
        return;

    for (size_t i = 0; i < table->size; i++)
    {
        struct alias_entry *entry = table->buckets[i];
        while (entry)
        {
            struct alias_entry *next = entry->next;
            free(entry->name);
            free(entry->value);
            free(entry);
            entry = next;
        }
    }

    free(table->buckets);
    free(table);
}

/* Helper for update existing alias */
static int update_existing_alias(struct alias_entry *entry, const char *value)
{
    char *new_value = strdup(value);
    if (!new_value)
        return -1;
    free(entry->value);
    entry->value = new_value;
    return 0;
}

static struct alias_entry *create_new_alias(const char *name, const char *value)
{
    struct alias_entry *new_entry = malloc(sizeof(struct alias_entry));
    if (!new_entry)
        return NULL;

    new_entry->name = strdup(name);
    new_entry->value = strdup(value);
    if (!new_entry->name || !new_entry->value)
    {
        free(new_entry->name);
        free(new_entry->value);
        free(new_entry);
        return NULL;
    }
    return new_entry;
}

/*
 * Handle alias table set
 * @param struct alias_table *table, const char *name,                    const
 * char *value
 * @return int
 */

int alias_table_set(struct alias_table *table, const char *name,
                    const char *value)
{
    if (!table || !name || !value)
        return -1;

    size_t index = hash_string(name);
    struct alias_entry *entry = table->buckets[index];

    while (entry)
    {
        if (strcmp(entry->name, name) == 0)
            return update_existing_alias(entry, value);
        entry = entry->next;
    }

    struct alias_entry *new_entry = create_new_alias(name, value);
    if (!new_entry)
        return -1;

    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    return 0;
}

const char *alias_table_get(struct alias_table *table, const char *name)
{
    if (!table || !name)
        return NULL;

    size_t index = hash_string(name);
    struct alias_entry *entry = table->buckets[index];

    while (entry)
    {
        if (strcmp(entry->name, name) == 0)
            return entry->value;
        entry = entry->next;
    }

    return NULL;
}

/*
 * Handle alias table unset
 * @param struct alias_table *table, const char *name
 * @return int
 */

int alias_table_unset(struct alias_table *table, const char *name)
{
    if (!table || !name)
        return -1;

    size_t index = hash_string(name);
    struct alias_entry **entry_ptr = &table->buckets[index];

    while (*entry_ptr)
    {
        struct alias_entry *entry = *entry_ptr;
        if (strcmp(entry->name, name) == 0)
        {
            *entry_ptr = entry->next;
            free(entry->name);
            free(entry->value);
            free(entry);
            return 0;
        }
        entry_ptr = &entry->next;
    }

    return -1;
}
