#include <stdio.h>
#include <string.h>

#include "builtins.h"

/* Print escaped */
static void print_escaped(const char *str)
{
    for (const char *p = str; *p; p++)
    {
        if (*p == '\\' && *(p + 1))
        {
            p++;
            if (*p == 'n')
                putchar('\n');
            else if (*p == 't')
                putchar('\t');
            else if (*p == '\\')
                putchar('\\');
            else
            {
                putchar('\\');
                putchar(*p);
            }
        }
        else
            putchar(*p);
    }
}

/* Check if echo option */
static int is_echo_option(const char *arg, int *newline, int *interpret_escapes)
{
    if (!arg || arg[0] != '-' || arg[1] == '\0')
        return 0;

    for (int i = 1; arg[i]; i++)
    {
        if (arg[i] != 'n' && arg[i] != 'e' && arg[i] != 'E')
            return 0;
    }

    for (int i = 1; arg[i]; i++)
    {
        if (arg[i] == 'n')
            *newline = 0;
        else if (arg[i] == 'e')
            *interpret_escapes = 1;
        else if (arg[i] == 'E')
            *interpret_escapes = 0;
    }

    return 1;
}

/* Parse echo options */
static int parse_echo_options(char **argv, int argc, int *newline,
                              int *interpret_escapes)
{
    int i = 1;
    while (i < argc && is_echo_option(argv[i], newline, interpret_escapes))
        i++;
    return i;
}

/*
 * Execute echo builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_echo(char **argv, int argc)
{
    int newline = 1;
    int interpret_escapes = 0;

    int i = parse_echo_options(argv, argc, &newline, &interpret_escapes);

    for (int j = i; j < argc; j++)
    {
        if (j > i)
            putchar(' ');
        if (interpret_escapes)
            print_escaped(argv[j]);
        else
            printf("%s", argv[j]);
    }

    if (newline)
        putchar('\n');

    fflush(stdout);
    return 0;
}
