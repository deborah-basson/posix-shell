#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "expansion.h"

/*
 * Expand argument vector
 * @param char **argv, int argc
 * @return char **
 */
char **expand_argv(char **argv, int argc)
{
    if (!argv || argc == 0)
        return NULL;

    char **expanded = malloc(sizeof(char *) * (argc + 1));
    if (!expanded)
        return NULL;

    for (int i = 0; i < argc; i++)
    {
        expanded[i] = expand_word(argv[i]);
        if (!expanded[i])
        {
            for (int j = 0; j < i; j++)
                free(expanded[j]);
            free(expanded);
            return NULL;
        }
    }
    expanded[argc] = NULL;
    return expanded;
}

/*
 * Free expanded argv
 * @param char **argv, int count
 * @return void
 */
void free_expanded_argv(char **argv, int count)
{
    if (!argv)
        return;

    for (int i = 0; i < count; i++)
    {
        if (argv[i])
            free(argv[i]);
    }
    free(argv);
}
