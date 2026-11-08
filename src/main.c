/* Main entry point - Shell REPL */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "builtins/builtins.h"
#include "exec/exec.h"
#include "exec/global_state.h"
#include "lexer/lexer.h"
#include "main_globals.h"
#include "parser/parser.h"
#include "pretty_print/pretty_print.h"
#include "utils/error.h"
#include "utils/utils.h"
#include "variables/variables.h"

struct main_globals g_main = { 0, NULL, 0 };

static char *empty_argv[] = { NULL };

static char *read_file(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *content = malloc(size + 1);
    if (!content)
    {
        fclose(f);
        return NULL;
    }
    size_t read = fread(content, 1, size, f);
    content[read] = '\0';
    fclose(f);
    return content;
}

static char *read_stdin(void)
{
    size_t size = 0;
    size_t capacity = 1024;
    char *content = malloc(capacity);
    if (!content)
        return NULL;
    int c;
    while ((c = fgetc(stdin)) != EOF)
    {
        if (size >= capacity - 1)
        {
            capacity *= 2;
            char *new_content = realloc(content, capacity);
            if (!new_content)
            {
                free(content);
                return NULL;
            }
            content = new_content;
        }
        content[size++] = c;
    }
    content[size] = '\0';
    return content;
}

/* Helper for usage */
static void usage(void)
{
    fprintf(stderr, "Usage: posix-shell [OPTIONS] [SCRIPT] [ARGUMENTS ...]\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -c <script>      Execute script from string\n");
    fprintf(stderr,
            "  --pretty-print   Parse and print the AST, do not execute\n");
}

/* Helper for strip pretty print */
static int strip_pretty_print(int argc, char **argv)
{
    int new_argc = 0;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--pretty-print") == 0)
            g_main.pretty_print = 1;
        else
            argv[new_argc++] = argv[i];
    }
    argv[new_argc] = NULL;
    return new_argc;
}

static char *handle_command_mode(int argc, char **argv)
{
    if (argc < 3)
        return (usage(), NULL);
    g_main.argv = (argc > 3) ? argv + 3 : empty_argv;
    g_main.argc = (argc > 3) ? argc - 3 : 0;
    return argv[2];
}

static char *handle_file_mode(int argc, char **argv, int *free_input)
{
    char *input = read_file(argv[1]);
    if (!input)
        fprintf(stderr, "posix-shell: cannot open %s\n", argv[1]);
    else
        *free_input = 1;
    g_main.argv = argv + 1;
    g_main.argc = argc - 1;
    return input;
}

static char *handle_stdin_mode(int *free_input)
{
    char *input = read_stdin();
    if (!input)
        fprintf(stderr, "posix-shell: failed to read input\n");
    else
        *free_input = 1;
    g_main.argv = empty_argv;
    g_main.argc = 0;
    return input;
}

static char *parse_arguments(int argc, char **argv, int *free_input)
{
    if (argc > 1 && strcmp(argv[1], "-c") == 0)
        return handle_command_mode(argc, argv);
    if (argc > 1)
        return handle_file_mode(argc, argv, free_input);
    /*
     * Handle handle stdin mode
     * @param free_input
     * @return return
     */

    return handle_stdin_mode(free_input);
}

/* Helper for execute shell */
static int execute_shell(char *input, int free_input)
{
    (void)free_input;
    struct lexer *lexer = lexer_new(input);
    if (!lexer)
        return 1;
    struct ast_node *ast = parse(lexer);
    if (!ast)
    {
        lexer_free(lexer);
        return 2;
    }
    int exit_status = 0;
    if (g_main.pretty_print)
        pretty_print(ast, 0);
    else
    {
        exit_status = exec_ast(ast);
        if (g_builtin_state.should_exit)
            exit_status = g_builtin_state.exit_code;
    }
    ast_free(ast);
    lexer_free(lexer);
    return exit_status;
}

/*
 * Handle main
 * @param int argc, char **argv
 * @return int
 */

int main(int argc, char **argv)
{
    srand(time(NULL));
    init_global_vars();
    init_global_state();

    argc = strip_pretty_print(argc, argv);

    int free_input = 0;
    char *input = parse_arguments(argc, argv, &free_input);
    if (!input)
    {
        cleanup_global_state();
        cleanup_global_vars();
        return (argc > 1 && strcmp(argv[1], "-c") == 0 && argc < 3) ? 2 : 1;
    }

    int exit_status = execute_shell(input, free_input);

    if (free_input)
        free(input);

    cleanup_global_vars();
    cleanup_global_state();

    return exit_status;
}
