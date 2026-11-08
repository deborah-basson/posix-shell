#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../exec/exec.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "builtins.h"

static char *try_path_in_token(const char *token, const char *filename)
{
    size_t len = strlen(token) + strlen(filename) + 2;
    char *full_path = malloc(len);
    if (!full_path)
        return NULL;

    snprintf(full_path, len, "%s/%s", token, filename);
    if (access(full_path, R_OK) == 0)
        return full_path;

    free(full_path);
    return NULL;
}

static char *find_in_path(const char *filename)
{
    if (strchr(filename, '/'))
        return (access(filename, R_OK) == 0) ? strdup(filename) : NULL;

    char *path_env = getenv("PATH");
    if (!path_env)
        return NULL;

    char *path = strdup(path_env);
    if (!path)
        return NULL;

    char *token = strtok(path, ":");
    while (token)
    {
        char *result = try_path_in_token(token, filename);
        if (result)
            return (free(path), result);
        token = strtok(NULL, ":");
    }

    free(path);
    return NULL;
}

static char *read_file_content(const char *filepath)
{
    FILE *f = fopen(filepath, "r");
    if (!f)
    {
        fprintf(stderr, "posix-shell: .: %s: cannot open\n", filepath);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *content = malloc(size + 1);
    if (!content)
    {
        fclose(f);
        return NULL;
    }

    size_t read_size = fread(content, 1, size, f);
    content[read_size] = '\0';
    fclose(f);
    return content;
}

/* Helper for execute file content */
static int execute_file_content(char *content)
{
    struct lexer *lexer = lexer_new(content);
    if (!lexer)
        return 1;

    struct ast_node *ast = parse(lexer);
    if (!ast)
    {
        lexer_free(lexer);
        return 2;
    }

    int status = exec_ast(ast);
    ast_free(ast);
    lexer_free(lexer);
    return status;
}

/*
 * Execute dot builtin
 * @param char **argv, int argc
 * @return int
 */

int builtin_dot(char **argv, int argc)
{
    if (argc < 2)
    {
        fprintf(stderr, "posix-shell: .: filename argument required\n");
        return 2;
    }

    char *filepath = find_in_path(argv[1]);
    if (!filepath)
    {
        fprintf(stderr, "posix-shell: .: %s: not found\n", argv[1]);
        return 1;
    }

    char *content = read_file_content(filepath);
    free(filepath);
    if (!content)
        return 1;

    int status = execute_file_content(content);
    free(content);
    return status;
}
