/* Variable expansion */

#define _POSIX_C_SOURCE 200809L
#include "expansion.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../ast/ast.h"
#include "../exec/exec.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../variables/variables.h"

char *expand_variable_at_pos(const char *word, size_t *pos);
char *expand_arithmetic(const char *word, size_t *pos);
char *expand_command_substitution(const char *word, size_t *pos);

struct expand_buffer
{
    char *result;
    size_t len;
    size_t capacity;
};

/* Execute command in child process */
static void execute_command_in_child(const char *command, int write_fd)
{
    dup2(write_fd, STDOUT_FILENO);
    close(write_fd);

    struct lexer *lexer = lexer_new(command);
    if (!lexer)
        exit(1);

    struct ast_node *ast = parse(lexer);
    if (!ast)
    {
        lexer_free(lexer);
        exit(2);
    }

    int status = exec_ast(ast);
    ast_free(ast);
    lexer_free(lexer);
    exit(status);
}

static char *grow_buffer(char *output, size_t *capacity)
{
    *capacity *= 2;
    char *new_out = realloc(output, *capacity);
    if (!new_out)
        free(output);
    return new_out;
}

static char *read_pipe_output(int read_fd, pid_t pid)
{
    size_t capacity = 4096;
    size_t total = 0;
    char *output = malloc(capacity);
    if (!output)
        return (close(read_fd), waitpid(pid, NULL, 0), strdup(""));

    char buf[4096];
    ssize_t n;
    while ((n = read(read_fd, buf, sizeof(buf))) > 0)
    {
        if (total + n >= capacity)
        {
            output = grow_buffer(output, &capacity);
            if (!output)
                return (close(read_fd), waitpid(pid, NULL, 0), strdup(""));
        }
        memcpy(output + total, buf, n);
        total += n;
    }

    output[total] = '\0';
    while (total > 0 && output[total - 1] == '\n')
        output[--total] = '\0';
    return output;
}

static char *handle_fork_error(int *pipefd)
{
    perror("fork");
    close(pipefd[0]);
    close(pipefd[1]);
    return strdup("");
}

static char *execute_and_capture(const char *command)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
        return (perror("pipe"), strdup(""));

    pid_t pid = fork();
    if (pid == -1)
        return handle_fork_error(pipefd);

    if (pid == 0)
    {
        close(pipefd[0]);
        execute_command_in_child(command, pipefd[1]);
    }

    close(pipefd[1]);
    char *output = read_pipe_output(pipefd[0], pid);
    close(pipefd[0]);
    waitpid(pid, NULL, 0);
    return output;
}

/* Skip single-quoted section */
static void skip_single_quote_in_cmd(const char *word, size_t *pos)
{
    (*pos)++;
    while (word[*pos] != '\0' && word[*pos] != '\'')
        (*pos)++;
    if (word[*pos] == '\'')
        (*pos)++;
}

/* Skip double-quoted section */
static void skip_double_quote_in_cmd(const char *word, size_t *pos)
{
    (*pos)++;
    while (word[*pos] != '\0' && word[*pos] != '"')
    {
        if (word[*pos] == '\\' && word[*pos + 1] != '\0')
            (*pos) += 2;
        else
            (*pos)++;
    }
    if (word[*pos] == '"')
        (*pos)++;
}

/* Update parenthesis depth */
static void update_depth(const char *word, size_t *pos, int *depth)
{
    if (word[*pos] == '(')
        (*depth)++;
    else if (word[*pos] == ')' && *depth > 0)
        (*depth)--;
    (*pos)++;
}

/* Find command substitution end */
static void find_cmd_end(const char *word, size_t *pos, int *depth)
{
    while (word[*pos] != '\0')
    {
        if (word[*pos] == '\'')
            skip_single_quote_in_cmd(word, pos);
        else if (word[*pos] == '"')
            skip_double_quote_in_cmd(word, pos);
        else if (word[*pos] == '(' || word[*pos] == ')')
            update_depth(word, pos, depth);
        else if (word[*pos] == ')' && *depth == 0)
            break;
        else
            (*pos)++;
    }
}

char *expand_command_substitution(const char *word, size_t *pos)
{
    if (word[*pos] != '$' || word[*pos + 1] != '(')
        return NULL;

    *pos += 2;
    size_t start = *pos;
    int depth = 0;

    find_cmd_end(word, pos, &depth);

    size_t len = *pos - start;
    char *command = malloc(len + 1);
    if (!command)
        return NULL;

    memcpy(command, word + start, len);
    command[len] = '\0';

    if (word[*pos] == ')')
        (*pos)++;

    char *result = execute_and_capture(command);
    free(command);
    return result;
}

/* Process variable expansion */
static void process_dollar(const char *word, size_t *pos,
                           struct expand_buffer *buf)
{
    char *expansion = expand_variable_at_pos(word, pos);
    if (!expansion)
        expansion = strdup("");
    buf->result =
        append_expansion(buf->result, &buf->len, &buf->capacity, expansion);
}

/* Append regular character */
static void process_char(const char *word, size_t *pos,
                         struct expand_buffer *buf)
{
    buf->result =
        append_char_expand(buf->result, &buf->len, &buf->capacity, word[*pos]);
    (*pos)++;
}

char *expand_word(const char *word)
{
    if (!word)
        return strdup("");

    struct expand_buffer buf = { NULL, 0, 256 };
    buf.result = malloc(buf.capacity);
    if (!buf.result)
        return NULL;

    size_t pos = 0;
    while (word[pos] != '\0')
    {
        if (word[pos] == '$')
            process_dollar(word, &pos, &buf);
        else
            process_char(word, &pos, &buf);

        if (!buf.result)
            return NULL;
    }

    buf.result[buf.len] = '\0';
    return buf.result;
}
