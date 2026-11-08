#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"
#include "ast.h"

struct for_node *for_node_new(const char *var)
{
    struct for_node *node = malloc(sizeof(struct for_node));
    if (!node)
        return NULL;
    node->variable = str_dup(var);
    node->words = NULL;
    node->word_count = 0;
    node->body = NULL;
    return node;
}

/*
 * Handle for node add word
 * @param struct for_node *node, const char *word
 * @return void
 */

void for_node_add_word(struct for_node *node, const char *word)
{
    node->words = realloc(node->words, sizeof(char *) * (node->word_count + 1));
    node->words[node->word_count++] = str_dup(word);
}

/*
 * Free for node
 * @param struct for_node *node
 * @return void
 */

void for_node_free(struct for_node *node)
{
    if (!node)
        return;
    free(node->variable);
    for (int i = 0; i < node->word_count; i++)
        free(node->words[i]);
    free(node->words);
    ast_release(node->body);
    free(node);
}
