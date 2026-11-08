/* Main entry point - Shell REPL */

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "expansion.h"
#include "field_splitting.h"

char *expand_arithmetic(const char *word, size_t *pos);
char *expand_command_substitution(const char *word, size_t *pos);
char *expand_variable_at_pos(const char *word, size_t *pos);
char *append_expansion(char *result, size_t *result_len, size_t *capacity,
                       char *expansion);
char *append_char_expand(char *result, size_t *result_len, size_t *capacity,
                         char c);

struct expand_ctx
{
    char *result;
    size_t *result_len;
    size_t *capacity;
    int in_squote;
    int in_dquote;
};

static char *append_with_quotes(struct expand_ctx *ctx, char *expansion)
{
    if (!expansion)
        return ctx->result;

    if (ctx->in_dquote)
    {
        ctx->result = append_char_expand(ctx->result, ctx->result_len,
                                         ctx->capacity, DQUOTE_START);
        if (!ctx->result)
            return (free(expansion), NULL);
    }

    ctx->result = append_expansion(ctx->result, ctx->result_len, ctx->capacity,
                                   expansion);
    if (!ctx->result)
        return NULL;

    if (ctx->in_dquote)
        ctx->result = append_char_expand(ctx->result, ctx->result_len,
                                         ctx->capacity, DQUOTE_END);
    return ctx->result;
}

static char *handle_escaped_dollar(struct expand_ctx *ctx, size_t *pos)
{
    ctx->result =
        append_char_expand(ctx->result, ctx->result_len, ctx->capacity, '$');
    *pos += 2;
    return ctx->result;
}

static char *handle_dollar_expansion(const char *word, struct expand_ctx *ctx,
                                     size_t *pos)
{
    char *expansion;

    if (word[*pos + 1] == '(')
    {
        if (word[*pos + 2] == '(')
            expansion = expand_arithmetic(word, pos);
        else
            expansion = expand_command_substitution(word, pos);
    }
    else
        expansion = expand_variable_at_pos(word, pos);

    /*
     * Handle append with quotes
     * @param ctx, expansion
     * @return return
     */

    return append_with_quotes(ctx, expansion);
}

static char *handle_squote(struct expand_ctx *ctx, size_t *pos, char marker)
{
    if (marker == SQUOTE_START)
        ctx->in_squote++;
    else
        ctx->in_squote--;
    ctx->result =
        append_char_expand(ctx->result, ctx->result_len, ctx->capacity, marker);
    (*pos)++;
    return ctx->result;
}

static char *handle_dquote(struct expand_ctx *ctx, size_t *pos, char marker)
{
    if (marker == DQUOTE_START)
        ctx->in_dquote++;
    else
        ctx->in_dquote--;
    (*pos)++;
    return ctx->result;
}

static char *process_word_char(const char *word, struct expand_ctx *ctx,
                               size_t *pos)
{
    if (word[*pos] == SQUOTE_START)
        return handle_squote(ctx, pos, SQUOTE_START);
    if (word[*pos] == SQUOTE_END)
        return handle_squote(ctx, pos, SQUOTE_END);
    if (word[*pos] == DQUOTE_START)
        return handle_dquote(ctx, pos, DQUOTE_START);
    if (word[*pos] == DQUOTE_END)
        return handle_dquote(ctx, pos, DQUOTE_END);
    if (word[*pos] == '\x01' && word[*pos + 1] == '$')
        return handle_escaped_dollar(ctx, pos);
    if (word[*pos] == '$' && !ctx->in_squote)
        return handle_dollar_expansion(word, ctx, pos);

    return append_char_expand(ctx->result, ctx->result_len, ctx->capacity,
                              word[(*pos)++]);
}

char *expand_word(const char *word)
{
    if (!word)
        return NULL;

    size_t capacity = strlen(word) + 128;
    char *result = malloc(capacity);
    if (!result)
        return NULL;

    size_t result_len = 0;
    size_t pos = 0;

    struct expand_ctx ctx = { result, &result_len, &capacity, 0, 0 };

    while (word[pos] != '\0')
    {
        ctx.result = process_word_char(word, &ctx, &pos);
        if (!ctx.result)
            return NULL;
    }

    ctx.result[result_len] = '\0';
    return ctx.result;
}
