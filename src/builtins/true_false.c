#include "builtins.h"

/*
 * Execute true builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_true(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 0;
}

/*
 * Execute false builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_false(char **argv, int argc)
{
    (void)argv;
    (void)argc;
    return 1;
}
