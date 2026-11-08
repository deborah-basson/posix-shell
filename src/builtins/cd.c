#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../variables/variables.h"
#include "builtins.h"

static char *get_target_directory(char **argv, int argc)
{
    if (argc == 1)
    {
        char *home = getenv("HOME");
        if (!home)
            fprintf(stderr, "posix-shell: cd: HOME not set\n");
        return home;
    }

    if (strcmp(argv[1], "-") == 0)
    {
        char *oldpwd = var_get(g_var_table, "OLDPWD");
        if (!oldpwd)
            fprintf(stderr, "posix-shell: cd: OLDPWD not set\n");
        else
            printf("%s\n", oldpwd);
        return oldpwd;
    }

    return argv[1];
}

/*
 * Execute cd builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_cd(char **argv, int argc)
{
    char *old_pwd = var_get(g_var_table, "PWD");
    char *target_dir = get_target_directory(argv, argc);

    if (!target_dir)
        return 1;

    if (chdir(target_dir) != 0)
        return (fprintf(stderr, "posix-shell: cd: %s: No such file or directory\n",
                        target_dir),
                1);

    if (old_pwd)
        var_set(g_var_table, "OLDPWD", old_pwd);

    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)))
        var_set(g_var_table, "PWD", cwd);

    return 0;
}
