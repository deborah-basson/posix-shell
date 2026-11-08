/* Main entry point - Shell REPL */

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "parser.h"

int token_is_redirection(enum token_type type);
int is_keyword_in_command_position(enum token_type type, int word_count);
struct redirection *parse_redirection(struct lexer *lexer);
struct redirection **add_redirection(struct redirection **redirs,
                                     int *redir_count,
                                     struct redirection *redir);
char **add_word(char **words, int *count, const char *value);
struct ast_node *try_parse_assignment(struct token *tok);
struct ast_node *build_simple_node(char **words, int count);
struct ast_node *build_redir_node(char **words, int count,
                                  struct redirection **redirs, int redir_count);

struct parse_ctx
{
    char ***words;
    int *count;
    struct redirection ***redirs;
    int *redir_count;
};

/* Helper for should parse as word */
static int should_parse_as_word(struct token *tok, int count)
{
    if (tok->type == TOKEN_WORD)
        return 1;

    if (count > 0 && is_keyword_in_command_position(tok->type, count))
        return 1;

    return 0;
}

static struct ast_node *
handle_first_word_assignment(struct token *tok, char **words,
                             struct redirection **redirs)
{
    struct ast_node *assign = try_parse_assignment(tok);
    if (assign)
    {
        free(words);
        free(redirs);
        return assign;
    }
    return NULL;
}

static struct ast_node *handle_redirection_token(struct lexer *lexer,
                                                 struct parse_ctx *ctx,
                                                 int *should_continue)
{
    struct redirection *redir = parse_redirection(lexer);
    if (!redir)
        return (*should_continue = 0, NULL);

    *ctx->redirs = add_redirection(*ctx->redirs, ctx->redir_count, redir);
    *should_continue = 1;
    return NULL;
}

static struct ast_node *handle_word_token(struct lexer *lexer,
                                          struct token *tok,
                                          struct parse_ctx *ctx,
                                          int *should_continue)
{
    if (*ctx->count == 0)
    {
        struct ast_node *assign =
            handle_first_word_assignment(tok, *ctx->words, *ctx->redirs);
        if (assign)
            return (lexer_next(lexer), assign);
    }
    *ctx->words = add_word(*ctx->words, ctx->count, tok->value);
    lexer_next(lexer);
    *should_continue = 1;
    return NULL;
}

static struct ast_node *parse_token_in_simple_command(struct lexer *lexer,
                                                      struct parse_ctx *ctx,
                                                      int *should_continue)
{
    struct token *tok = lexer_peek(lexer);
    if (!tok)
        return (*should_continue = 0, NULL);

    if (token_is_redirection(tok->type))
        return handle_redirection_token(lexer, ctx, should_continue);

    if (should_parse_as_word(tok, *ctx->count))
        return handle_word_token(lexer, tok, ctx, should_continue);

    *should_continue = 0;
    return NULL;
}

struct ast_node *parse_simple_command(struct lexer *lexer)
{
    char **words = NULL;
    int count = 0;
    struct redirection **redirs = NULL;
    int redir_count = 0;

    struct parse_ctx ctx = { &words, &count, &redirs, &redir_count };

    while (1)
    {
        int should_continue = 0;
        struct ast_node *result =
            parse_token_in_simple_command(lexer, &ctx, &should_continue);
        if (result)
            return result;
        if (!should_continue)
            break;
    }

    if (count == 0 && redir_count == 0)
    {
        free(words);
        free(redirs);
        return NULL;
    }
    if (redir_count > 0)
        return build_redir_node(words, count, redirs, redir_count);
    return build_simple_node(words, count);
}
