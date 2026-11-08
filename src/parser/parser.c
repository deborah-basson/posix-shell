/* Parser - Token stream to AST */

#define _POSIX_C_SOURCE 200809L
#include "parser.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../variables/variables.h"

static int g_parser_error = 0;

/*
 * Set parser error flag
 * @param const char *msg
 * @return void
 */

void parser_error(const char *msg)
{
    (void)msg;
    g_parser_error = 1;
}

/*
 * Check parser error
 * @param void
 * @return int
 */

int parser_had_error(void)
{
    return g_parser_error;
}

/*
 * Reset parser error
 * @param void
 * @return void
 */

void parser_reset_error(void)
{
    g_parser_error = 0;
}

static struct ast_node *parse_compound_list(struct lexer *lexer);
static struct ast_node *parse_condition(struct lexer *lexer);
static struct ast_node *parse_command(struct lexer *lexer);
static struct ast_node *parse_pipeline(struct lexer *lexer);
struct ast_node *parse_and_or(struct lexer *lexer);
struct ast_node *parse_simple_command(struct lexer *lexer);

/*
 * Check token type match
 * @param struct lexer *lexer, enum token_type type
 * @return int
 */

int match(struct lexer *lexer, enum token_type type)
{
    struct token *token = lexer_peek(lexer);
    return token && token->type == type;
}

static struct token *expect(struct lexer *lexer, enum token_type type)
{
    struct token *token = lexer_peek(lexer);
    if (!token || token->type != type)
        return NULL;
    lexer_next(lexer);
    return token;
}

/*
 * Skip newline tokens
 * @param struct lexer *lexer
 * @return void
 */

void skip_newlines(struct lexer *lexer)
{
    while (match(lexer, TOKEN_NEWLINE))
        lexer_next(lexer);
}

struct redirection *parse_redirection(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (!token_is_redirection(tok->type))
        return NULL;

    enum token_type redir_type = tok->type;
    int io_number = tok->io_number;
    lexer_next(lexer);

    if (!match(lexer, TOKEN_WORD))
    {
        parser_error("expected word after redirection");
        return NULL;
    }

    struct token *target = lexer_peek(lexer);
    struct redirection *redir =
        redirection_new(redir_type, io_number, target->value);
    lexer_next(lexer);

    return redir;
}

/*
 * Handle is assignment
 * @param const char *word
 * @return int
 */

int is_assignment(const char *word)
{
    if (!word || !*word)
        return 0;

    if (!isalpha(*word) && *word != '_')
        return 0;

    const char *p = word;
    while (*p && *p != '=')
    {
        if (!isalnum(*p) && *p != '_')
            return 0;
        p++;
    }

    return *p == '=';
}

static struct ast_node *create_empty_block(void)
{
    struct command_block *block = malloc(sizeof(struct command_block));
    if (!block)
        return NULL;
    block->commands = NULL;
    return ast_node_new(AST_COMMAND_BLOCK, block);
}

static struct ast_node *create_block_with_commands(struct ast_node *commands)
{
    struct command_block *block = malloc(sizeof(struct command_block));
    if (!block)
    {
        ast_free(commands);
        return NULL;
    }
    block->commands = commands;
    return ast_node_new(AST_COMMAND_BLOCK, block);
}

static struct ast_node *parse_command_block(struct lexer *lexer)
{
    if (!expect(lexer, TOKEN_LBRACE))
        return NULL;

    if (match(lexer, TOKEN_RBRACE))
    {
        lexer_next(lexer);
        return create_empty_block();
    }

    struct ast_node *commands = parse_compound_list(lexer);
    if (!commands || !expect(lexer, TOKEN_RBRACE))
    {
        ast_free(commands);
        return NULL;
    }

    return create_block_with_commands(commands);
}

/* Expand elif array */
static bool grow_elif_parts(struct if_node *if_node)
{
    struct elif_clause *new_elifs =
        realloc(if_node->elif_parts,
                sizeof(struct elif_clause) * (if_node->elif_count + 1));
    if (!new_elifs)
        return false;
    if_node->elif_parts = new_elifs;
    return true;
}

/* Parse single elif clause */
static bool parse_elif_clause(struct lexer *lexer, struct if_node *if_node)
{
    lexer_next(lexer);

    if (!grow_elif_parts(if_node))
        return false;

    if_node->elif_parts[if_node->elif_count].condition = parse_condition(lexer);
    if (!if_node->elif_parts[if_node->elif_count].condition)
        return (parser_error("expected condition after 'elif'"), false);

    if (!expect(lexer, TOKEN_THEN))
        return (parser_error("expected 'then'"), false);

    if_node->elif_parts[if_node->elif_count].then_part =
        parse_compound_list(lexer);
    if (!if_node->elif_parts[if_node->elif_count].then_part)
        return false;

    if_node->elif_count++;
    return true;
}

/* Parse all elif clauses */
static bool parse_elif_clauses(struct lexer *lexer, struct if_node *if_node)
{
    while (match(lexer, TOKEN_ELIF))
    {
        if (!parse_elif_clause(lexer, if_node))
            return false;
    }
    return true;
}

/* Parse else clause */
static bool parse_else_clause(struct lexer *lexer, struct if_node *if_node)
{
    if (!match(lexer, TOKEN_ELSE))
        return true;

    lexer_next(lexer);
    if_node->else_part = parse_compound_list(lexer);
    return if_node->else_part != NULL;
}

/* Parse if-then structure */
static bool parse_if_then(struct lexer *lexer, struct if_node *if_node)
{
    if_node->condition = parse_condition(lexer);
    if (!if_node->condition)
    {
        parser_error("expected condition after 'if'");
        return false;
    }

    if (!expect(lexer, TOKEN_THEN))
    {
        parser_error("expected 'then'");
        return false;
    }

    if_node->then_part = parse_compound_list(lexer);
    return if_node->then_part != NULL;
}

static struct ast_node *parse_if(struct lexer *lexer)
{
    if (!expect(lexer, TOKEN_IF))
        return NULL;

    struct if_node *if_node = if_node_new();
    if (!if_node)
        return NULL;

    if (!parse_if_then(lexer, if_node) || !parse_elif_clauses(lexer, if_node)
        || !parse_else_clause(lexer, if_node))
    {
        if_node_free(if_node);
        return NULL;
    }

    if (!expect(lexer, TOKEN_FI))
    {
        parser_error("expected 'fi'");
        if_node_free(if_node);
        return NULL;
    }

    return ast_node_new(AST_IF, if_node);
}

static struct ast_node *parse_loop_body(struct lexer *lexer,
                                        struct ast_node *condition)
{
    if (!expect(lexer, TOKEN_DO))
    {
        parser_error("expected 'do'");
        ast_free(condition);
        return NULL;
    }

    struct ast_node *body = parse_compound_list(lexer);
    if (!body)
    {
        ast_free(condition);
        return NULL;
    }

    if (!expect(lexer, TOKEN_DONE))
    {
        parser_error("expected 'done'");
        ast_free(condition);
        ast_free(body);
        return NULL;
    }

    return body;
}

static struct ast_node *parse_while(struct lexer *lexer)
{
    if (!expect(lexer, TOKEN_WHILE))
        return NULL;

    struct ast_node *condition = parse_condition(lexer);
    if (!condition)
    {
        parser_error("expected condition after 'while'");
        return NULL;
    }

    struct ast_node *body = parse_loop_body(lexer, condition);
    if (!body)
        return NULL;

    struct loop_node *loop = loop_node_new(condition, body);
    return ast_node_new(AST_WHILE, loop);
}

static struct ast_node *parse_until(struct lexer *lexer)
{
    if (!expect(lexer, TOKEN_UNTIL))
        return NULL;

    struct ast_node *condition = parse_condition(lexer);
    if (!condition)
    {
        parser_error("expected condition after 'until'");
        return NULL;
    }

    struct ast_node *body = parse_loop_body(lexer, condition);
    if (!body)
        return NULL;

    struct loop_node *loop = loop_node_new(condition, body);
    return ast_node_new(AST_UNTIL, loop);
}

/* Parse for loop word list */
static void parse_for_words(struct lexer *lexer, struct for_node *for_node)
{
    if (!match(lexer, TOKEN_IN))
        return;

    lexer_next(lexer);
    while (match(lexer, TOKEN_WORD))
    {
        struct token *word = lexer_peek(lexer);
        for_node_add_word(for_node, word->value);
        lexer_next(lexer);
    }
}

/* Parse separator after for */
static int parse_for_separator(struct lexer *lexer)
{
    if (match(lexer, TOKEN_SEMICOLON))
        lexer_next(lexer);
    else if (match(lexer, TOKEN_NEWLINE))
        lexer_next(lexer);

    skip_newlines(lexer);
    return 1;
}

static struct for_node *init_for_node(struct lexer *lexer)
{
    if (!match(lexer, TOKEN_WORD))
    {
        parser_error("expected variable name after 'for'");
        return NULL;
    }
    struct token *var_tok = lexer_peek(lexer);
    struct for_node *for_node = for_node_new(var_tok->value);
    lexer_next(lexer);
    parse_for_words(lexer, for_node);
    parse_for_separator(lexer);
    return for_node;
}

static struct ast_node *parse_for(struct lexer *lexer)
{
    if (!expect(lexer, TOKEN_FOR))
        return NULL;

    struct for_node *for_node = init_for_node(lexer);
    if (!for_node)
        return NULL;

    if (!expect(lexer, TOKEN_DO))
        return (parser_error("expected 'do'"), for_node_free(for_node), NULL);

    for_node->body = parse_compound_list(lexer);
    if (!for_node->body)
        return (for_node_free(for_node), NULL);

    if (!expect(lexer, TOKEN_DONE))
        return (parser_error("expected 'done'"), for_node_free(for_node), NULL);

    return ast_node_new(AST_FOR, for_node);
}

/* Check if more case items */
static int should_parse_more_items(struct token *current)
{
    return current && current->type != TOKEN_ESAC;
}

/* Skip newlines and peek token */
static void skip_newlines_peek(struct lexer *lexer, struct token **current)
{
    *current = lexer_peek(lexer);
    while (*current && (*current)->type == TOKEN_NEWLINE)
    {
        lexer_next(lexer);
        *current = lexer_peek(lexer);
    }
}

static struct case_item *create_case_item(char *pattern,
                                          struct ast_node *commands)
{
    struct case_item *item = malloc(sizeof(struct case_item));
    if (!item)
    {
        free(pattern);
        ast_free(commands);
        return NULL;
    }
    item->pattern = pattern;
    item->commands = commands;
    item->next = NULL;
    return item;
}

static struct case_item *parse_case_item(struct lexer *lexer)
{
    struct token *current = lexer_peek(lexer);
    if (!current)
        return NULL;
    if (current->type == TOKEN_LPAREN)
        lexer_next(lexer);
    current = lexer_peek(lexer);
    if (!current || current->type != TOKEN_WORD)
        return NULL;
    char *pattern = strdup(current->value);
    lexer_next(lexer);
    current = lexer_peek(lexer);
    if (!current || current->type != TOKEN_RPAREN)
        return (free(pattern), NULL);
    lexer_next(lexer);
    skip_newlines_peek(lexer, &current);
    struct ast_node *commands = NULL;
    if (current && current->type != TOKEN_DSEMI && current->type != TOKEN_ESAC)
        commands = parse_compound_list(lexer);
    current = lexer_peek(lexer);
    if (current && current->type == TOKEN_DSEMI)
        lexer_next(lexer);
    return create_case_item(pattern, commands);
}

/* Add item to case list */
static void add_case_item(struct case_item **items, struct case_item **tail,
                          struct case_item *item)
{
    if (!*items)
    {
        *items = item;
        *tail = item;
    }
    else
    {
        (*tail)->next = item;
        *tail = item;
    }
}

static struct case_item *parse_case_items(struct lexer *lexer)
{
    struct case_item *items = NULL;
    struct case_item *tail = NULL;
    struct token *current = lexer_peek(lexer);

    while (should_parse_more_items(current))
    {
        struct case_item *item = parse_case_item(lexer);
        if (!item)
            break;
        add_case_item(&items, &tail, item);
        skip_newlines_peek(lexer, &current);
    }
    return items;
}

static char *parse_case_word(struct lexer *lexer)
{
    struct token *current = lexer_peek(lexer);
    if (!current || current->type != TOKEN_WORD)
    {
        fprintf(stderr, "posix-shell: syntax error: expected word after 'case'\n");
        return NULL;
    }
    char *word = strdup(current->value);
    lexer_next(lexer);
    return word;
}

/* Parse IN keyword */
static int parse_case_in_keyword(struct lexer *lexer, char *word)
{
    struct token *current;
    skip_newlines_peek(lexer, &current);

    if (!current || current->type != TOKEN_IN)
    {
        fprintf(stderr, "posix-shell: syntax error: expected 'in' after case word\n");
        free(word);
        return 0;
    }
    lexer_next(lexer);
    return 1;
}

static struct ast_node *finish_case(struct lexer *lexer, char *word,
                                    struct case_item *items)
{
    struct token *current = lexer_peek(lexer);
    if (!current || current->type != TOKEN_ESAC)
        return (fprintf(stderr, "posix-shell: syntax error\n"), free(word), NULL);

    lexer_next(lexer);
    struct case_node *cn = malloc(sizeof(struct case_node));
    if (!cn)
        return (free(word), NULL);
    cn->word = word;
    cn->items = items;
    return ast_node_new(AST_CASE, cn);
}

static struct ast_node *parse_case(struct lexer *lexer)
{
    struct token *current = lexer_peek(lexer);
    if (!current || current->type != TOKEN_CASE)
        return NULL;

    lexer_next(lexer);
    char *word = parse_case_word(lexer);
    if (!word || !parse_case_in_keyword(lexer, word))
        return NULL;

    skip_newlines_peek(lexer, &current);
    struct case_item *items = parse_case_items(lexer);
    return finish_case(lexer, word, items);
}

/* Check if function definition */
static int is_function_definition(struct lexer *lexer)
{
    if (!match(lexer, TOKEN_WORD))
        return 0;

    size_t saved_pos = lexer->pos;
    struct token *saved_token = lexer->current;
    lexer->current = NULL;

    lexer_next(lexer);
    int is_func = match(lexer, TOKEN_LPAREN);

    lexer->pos = saved_pos;
    token_free(lexer->current);
    lexer->current = saved_token;

    return is_func;
}

static struct ast_node *parse_shell_command(struct lexer *lexer)
{
    if (match(lexer, TOKEN_IF))
        return parse_if(lexer);
    if (match(lexer, TOKEN_WHILE))
        return parse_while(lexer);
    if (match(lexer, TOKEN_UNTIL))
        return parse_until(lexer);
    if (match(lexer, TOKEN_FOR))
        return parse_for(lexer);
    if (match(lexer, TOKEN_CASE))
        return parse_case(lexer);
    if (match(lexer, TOKEN_LBRACE))
        return parse_command_block(lexer);

    return parse_simple_command(lexer);
}

/* Parse function redirections */
static void parse_func_redirections(struct lexer *lexer,
                                    struct function_def_node *func_def)
{
    while (lexer_peek(lexer) && token_is_redirection(lexer_peek(lexer)->type))
    {
        struct redirection *redir = parse_redirection(lexer);
        if (!redir)
            break;
        func_def->redirs =
            realloc(func_def->redirs,
                    sizeof(struct redirection *) * (func_def->redir_count + 1));
        func_def->redirs[func_def->redir_count++] = redir;
    }
}

static char *parse_func_header(struct lexer *lexer)
{
    struct token *name_token = lexer_peek(lexer);
    char *func_name = strdup(name_token->value);
    lexer_next(lexer);

    if (!expect(lexer, TOKEN_LPAREN))
        return (parser_error("expected '('"), free(func_name), NULL);
    if (!expect(lexer, TOKEN_RPAREN))
        return (parser_error("expected ')'"), free(func_name), NULL);
    return func_name;
}

static struct ast_node *parse_function_def(struct lexer *lexer)
{
    char *func_name = parse_func_header(lexer);
    if (!func_name)
        return NULL;

    skip_newlines(lexer);
    struct ast_node *body = parse_shell_command(lexer);
    if (!body)
        return (parser_error("expected function body"), free(func_name), NULL);

    struct function_def_node *func_def = function_def_node_new(func_name, body);
    free(func_name);
    if (!func_def)
        return (ast_release(body), NULL);

    parse_func_redirections(lexer, func_def);
    return ast_node_new(AST_FUNCTION_DEF, func_def);
}

static struct ast_node *parse_subshell(struct lexer *lexer)
{
    if (!expect(lexer, TOKEN_LPAREN))
        return NULL;

    skip_newlines(lexer);

    struct ast_node *command = parse_compound_list(lexer);
    if (!command)
    {
        parser_error("expected command in subshell");
        return NULL;
    }

    if (!expect(lexer, TOKEN_RPAREN))
    {
        parser_error("expected ')'");
        ast_free(command);
        return NULL;
    }

    struct subshell_node *subshell = subshell_node_new(command);
    return ast_node_new(AST_SUBSHELL, subshell);
}

static struct ast_node *parse_brace_group(struct lexer *lexer)
{
    if (!expect(lexer, TOKEN_LBRACE))
        return NULL;

    skip_newlines(lexer);
    struct ast_node *command = parse_compound_list(lexer);
    skip_newlines(lexer);

    if (!command || !expect(lexer, TOKEN_RBRACE))
    {
        ast_free(command);
        return NULL;
    }

    struct brace_group_node *bg = brace_group_node_new(command);
    return ast_node_new(AST_BRACE_GROUP, bg);
}

static struct ast_node *parse_command(struct lexer *lexer)
{
    if (is_function_definition(lexer))
        return parse_function_def(lexer);
    if (match(lexer, TOKEN_IF))
        return parse_if(lexer);
    if (match(lexer, TOKEN_WHILE))
        return parse_while(lexer);
    if (match(lexer, TOKEN_UNTIL))
        return parse_until(lexer);
    if (match(lexer, TOKEN_FOR))
        return parse_for(lexer);
    if (match(lexer, TOKEN_LPAREN))
        return parse_subshell(lexer);
    if (match(lexer, TOKEN_LBRACE))
        return parse_brace_group(lexer);
    if (match(lexer, TOKEN_CASE))
        return parse_case(lexer);
    return parse_simple_command(lexer);
}

static struct ast_node *apply_negations(struct ast_node *cmd, int count)
{
    for (int i = 0; i < count; i++)
    {
        struct negation_node *neg = negation_node_new(cmd);
        cmd = ast_node_new(AST_NEGATION, neg);
    }
    return cmd;
}

static struct ast_node *parse_pipeline_commands(struct lexer *lexer,
                                                struct ast_node *first_cmd)
{
    struct pipeline_node *pipeline = pipeline_node_new();
    pipeline_node_add(pipeline, first_cmd);

    while (match(lexer, TOKEN_PIPE))
    {
        lexer_next(lexer);
        skip_newlines(lexer);

        struct ast_node *cmd = parse_command(lexer);
        if (!cmd)
        {
            parser_error("expected command after '|'");
            pipeline_node_free(pipeline);
            return NULL;
        }
        pipeline_node_add(pipeline, cmd);
    }

    return ast_node_new(AST_PIPELINE, pipeline);
}

static struct ast_node *parse_pipeline(struct lexer *lexer)
{
    int negation_count = 0;
    while (match(lexer, TOKEN_BANG))
    {
        negation_count++;
        lexer_next(lexer);
    }

    struct ast_node *cmd = parse_command(lexer);
    if (!cmd)
        return NULL;

    if (!match(lexer, TOKEN_PIPE))
        return apply_negations(cmd, negation_count);

    cmd = parse_pipeline_commands(lexer, cmd);
    if (!cmd)
        return NULL;

    return apply_negations(cmd, negation_count);
}

struct ast_node *parse_and_or(struct lexer *lexer)
{
    struct ast_node *left = parse_pipeline(lexer);
    if (!left)
        return NULL;

    while (match(lexer, TOKEN_AND_IF) || match(lexer, TOKEN_OR_IF))
    {
        int is_and = match(lexer, TOKEN_AND_IF);
        lexer_next(lexer);
        skip_newlines(lexer);

        struct ast_node *right = parse_pipeline(lexer);
        if (!right)
        {
            parser_error("expected command after '&&' or '||'");
            ast_free(left);
            return NULL;
        }

        struct and_or_node *node = and_or_node_new(left, right, is_and);
        left = ast_node_new(AST_AND_OR, node);
    }

    return left;
}

/* Check if list terminator */
static int is_compound_list_terminator(struct lexer *lexer)
{
    return match(lexer, TOKEN_THEN) || match(lexer, TOKEN_ELIF)
        || match(lexer, TOKEN_ELSE) || match(lexer, TOKEN_FI)
        || match(lexer, TOKEN_DO) || match(lexer, TOKEN_DONE)
        || match(lexer, TOKEN_EOF) || match(lexer, TOKEN_RPAREN)
        || match(lexer, TOKEN_RBRACE);
}

/* Skip separator tokens */
static void skip_separators(struct lexer *lexer)
{
    while (match(lexer, TOKEN_SEMICOLON) || match(lexer, TOKEN_NEWLINE))
    {
        lexer_next(lexer);
        if (match(lexer, TOKEN_NEWLINE) || match(lexer, TOKEN_SEMICOLON))
            skip_newlines(lexer);
    }
}

/* Add commands to list */
static int add_commands_to_list(struct lexer *lexer, struct list_node *list)
{
    struct ast_node *cmd = parse_and_or(lexer);
    if (!cmd)
    {
        if (match(lexer, TOKEN_RPAREN) || match(lexer, TOKEN_RBRACE))
            return 1;
        return 0;
    }

    list_node_add(list, cmd);
    while (match(lexer, TOKEN_SEMICOLON) || match(lexer, TOKEN_NEWLINE))
    {
        lexer_next(lexer);
        skip_newlines(lexer);
        if (is_compound_list_terminator(lexer))
            break;
        cmd = parse_and_or(lexer);
        if (!cmd)
            break;
        list_node_add(list, cmd);
    }
    return 1;
}

static struct ast_node *parse_compound_list(struct lexer *lexer)
{
    skip_newlines(lexer);
    struct list_node *list = list_node_new();
    if (!list)
        return NULL;

    skip_separators(lexer);
    if (is_compound_list_terminator(lexer))
        return ast_node_new(AST_COMPOUND_LIST, list);

    if (!add_commands_to_list(lexer, list))
    {
        list_node_free(list);
        return NULL;
    }

    return ast_node_new(AST_COMPOUND_LIST, list);
}

static struct ast_node *parse_condition(struct lexer *lexer)
{
    skip_newlines(lexer);
    struct list_node *list = list_node_new();
    if (!list)
        return NULL;

    skip_separators(lexer);
    if (is_compound_list_terminator(lexer))
    {
        list_node_free(list);
        return NULL;
    }

    if (!add_commands_to_list(lexer, list) || list->count == 0)
    {
        list_node_free(list);
        return NULL;
    }

    return ast_node_new(AST_COMPOUND_LIST, list);
}

/* Skip list separators */
static void skip_list_separators(struct lexer *lexer)
{
    while (match(lexer, TOKEN_SEMICOLON) || match(lexer, TOKEN_NEWLINE))
        lexer_next(lexer);
}

/* Check if valid command start */
static int is_valid_command_start(struct lexer *lexer)
{
    return match(lexer, TOKEN_WORD) || match(lexer, TOKEN_IF)
        || match(lexer, TOKEN_WHILE) || match(lexer, TOKEN_UNTIL)
        || match(lexer, TOKEN_FOR) || match(lexer, TOKEN_LBRACE)
        || match(lexer, TOKEN_LPAREN) || match(lexer, TOKEN_BANG)
        || (lexer_peek(lexer) && token_is_redirection(lexer_peek(lexer)->type));
}

/* Validate list start */
static int check_list_start(struct lexer *lexer, struct list_node *list)
{
    if (match(lexer, TOKEN_SEMICOLON))
    {
        parser_error("unexpected token ';'");
        list_node_free(list);
        return -1;
    }
    skip_list_separators(lexer);
    if (match(lexer, TOKEN_EOF))
        return 0;
    return 1;
}

/* Parse remaining commands */
static int parse_additional_commands(struct lexer *lexer,
                                     struct list_node *list)
{
    while (match(lexer, TOKEN_SEMICOLON) || match(lexer, TOKEN_NEWLINE))
    {
        lexer_next(lexer);
        skip_list_separators(lexer);
        if (match(lexer, TOKEN_EOF))
            break;
        if (!is_valid_command_start(lexer))
            return (parser_error("unexpected token"), -1);
        struct ast_node *cmd = parse_and_or(lexer);
        if (cmd)
            list_node_add(list, cmd);
    }
    return 0;
}

static struct ast_node *parse_list(struct lexer *lexer)
{
    struct list_node *list = list_node_new();
    if (!list)
        return NULL;

    int status = check_list_start(lexer, list);
    if (status == -1)
        return NULL;
    if (status == 0)
        return ast_node_new(AST_LIST, list);

    if (!is_valid_command_start(lexer))
        return (parser_error("unexpected token"), list_node_free(list), NULL);

    struct ast_node *cmd = parse_and_or(lexer);
    if (!cmd)
        return (list_node_free(list), NULL);

    list_node_add(list, cmd);
    if (parse_additional_commands(lexer, list) == -1)
        return (list_node_free(list), NULL);

    return ast_node_new(AST_LIST, list);
}

struct ast_node *parse(struct lexer *lexer)
{
    g_parser_error = 0;
    skip_newlines(lexer);

    if (match(lexer, TOKEN_EOF))
    {
        struct list_node *empty_list = list_node_new();
        return ast_node_new(AST_LIST, empty_list);
    }

    struct ast_node *ast = parse_list(lexer);
    if (g_parser_error)
    {
        ast_free(ast);
        return NULL;
    }
    return ast;
}
