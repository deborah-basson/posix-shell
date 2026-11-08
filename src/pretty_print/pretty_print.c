#define _POSIX_C_SOURCE 200809L
#include "pretty_print.h"

#include <stdio.h>

#include "../lexer/token.h"

/* Print indentation spaces */
static void print_indent(int indent)
{
    for (int i = 0; i < indent; i++)
        fputs("  ", stdout);
}

static const char *redir_type_str(enum token_type type)
{
    switch (type)
    {
    case TOKEN_REDIR_IN:
        return "<";
    case TOKEN_REDIR_OUT:
        return ">";
    case TOKEN_REDIR_APPEND:
        return ">>";
    case TOKEN_REDIR_DUP_OUT:
        return ">&";
    case TOKEN_REDIR_DUP_IN:
        return "<&";
    case TOKEN_REDIR_CLOBBER:
        return ">|";
    case TOKEN_REDIR_RDWR:
        return "<>";
    default:
        return "?";
    }
}

/* Print redirection list */
static void print_redirs(struct redirection **redirs, int count)
{
    for (int i = 0; i < count; i++)
    {
        struct redirection *r = redirs[i];
        if (r->io_number >= 0)
            printf(" %d%s%s", r->io_number, redir_type_str(r->redir_type),
                   r->target);
        else
            printf(" %s%s", redir_type_str(r->redir_type), r->target);
    }
}

static void pp_node(const struct ast_node *node, int indent);

/* Print simple command */
static void pp_simple_command(const struct simple_command *cmd, int indent)
{
    print_indent(indent);
    fputs("command", stdout);
    for (int i = 0; i < cmd->argc; i++)
        printf(" \"%s\"", cmd->argv[i]);
    putchar('\n');
}

/* Print command list */
static void pp_list(const struct list_node *list, int indent)
{
    print_indent(indent);
    fputs("list {\n", stdout);
    for (int i = 0; i < list->count; i++)
        pp_node(list->commands[i], indent + 1);
    print_indent(indent);
    fputs("}\n", stdout);
}

/* Print if statement */
static void pp_if(const struct if_node *node, int indent)
{
    print_indent(indent);
    fputs("if {\n", stdout);
    print_indent(indent + 1);
    fputs("condition:\n", stdout);
    pp_node(node->condition, indent + 2);
    print_indent(indent + 1);
    fputs("then:\n", stdout);
    pp_node(node->then_part, indent + 2);
    for (int i = 0; i < node->elif_count; i++)
    {
        print_indent(indent + 1);
        fputs("elif:\n", stdout);
        pp_node(node->elif_parts[i].condition, indent + 2);
        print_indent(indent + 1);
        fputs("then:\n", stdout);
        pp_node(node->elif_parts[i].then_part, indent + 2);
    }
    if (node->else_part)
    {
        print_indent(indent + 1);
        fputs("else:\n", stdout);
        pp_node(node->else_part, indent + 2);
    }
    print_indent(indent);
    fputs("}\n", stdout);
}

/* Print redirection */
static void pp_redir(const struct command_with_redirs *node, int indent)
{
    print_indent(indent);
    fputs("redir [", stdout);
    print_redirs(node->redirs, node->redir_count);
    fputs(" ] {\n", stdout);
    pp_node(node->command, indent + 1);
    print_indent(indent);
    fputs("}\n", stdout);
}

/* Print pipeline */
static void pp_pipeline(const struct pipeline_node *node, int indent)
{
    print_indent(indent);
    fputs("pipeline {\n", stdout);
    for (int i = 0; i < node->count; i++)
        pp_node(node->commands[i], indent + 1);
    print_indent(indent);
    fputs("}\n", stdout);
}

/* Print negation */
static void pp_negation(const struct negation_node *node, int indent)
{
    print_indent(indent);
    fputs("! {\n", stdout);
    pp_node(node->command, indent + 1);
    print_indent(indent);
    fputs("}\n", stdout);
}

/* Print loop structure */
static void pp_loop(const struct loop_node *node, int indent,
                    const char *keyword)
{
    print_indent(indent);
    printf("%s {\n", keyword);
    print_indent(indent + 1);
    fputs("condition:\n", stdout);
    pp_node(node->condition, indent + 2);
    print_indent(indent + 1);
    fputs("body:\n", stdout);
    pp_node(node->body, indent + 2);
    print_indent(indent);
    fputs("}\n", stdout);
}

/* Print AND/OR operator */
static void pp_and_or(const struct and_or_node *node, int indent)
{
    print_indent(indent);
    printf("%s {\n", node->is_and ? "&&" : "||");
    pp_node(node->left, indent + 1);
    pp_node(node->right, indent + 1);
    print_indent(indent);
    fputs("}\n", stdout);
}

/* Print for loop */
static void pp_for(const struct for_node *node, int indent)
{
    print_indent(indent);
    printf("for %s in", node->variable);
    for (int i = 0; i < node->word_count; i++)
        printf(" \"%s\"", node->words[i]);
    fputs(" {\n", stdout);
    pp_node(node->body, indent + 1);
    print_indent(indent);
    fputs("}\n", stdout);
}

/* Print assignment */
static void pp_assignment(const struct assignment_node *node, int indent)
{
    print_indent(indent);
    printf("assign %s=%s\n", node->name, node->value ? node->value : "");
}

/* Print function definition */
static void pp_function_def(const struct function_def_node *node, int indent)
{
    print_indent(indent);
    printf("function %s", node->name);
    if (node->redir_count > 0)
        print_redirs(node->redirs, node->redir_count);
    fputs(" {\n", stdout);
    pp_node(node->body, indent + 1);
    print_indent(indent);
    fputs("}\n", stdout);
}

/* Print subshell */
static void pp_subshell(const struct subshell_node *node, int indent)
{
    print_indent(indent);
    fputs("subshell (\n", stdout);
    pp_node(node->command, indent + 1);
    print_indent(indent);
    fputs(")\n", stdout);
}

/* Print brace group */
static void pp_brace_group(const struct brace_group_node *node, int indent)
{
    print_indent(indent);
    fputs("brace_group {\n", stdout);
    pp_node(node->command, indent + 1);
    print_indent(indent);
    fputs("}\n", stdout);
}

/* Print case statement */
static void pp_case(const struct case_node *node, int indent)
{
    print_indent(indent);
    printf("case \"%s\" {\n", node->word);
    for (struct case_item *item = node->items; item; item = item->next)
    {
        print_indent(indent + 1);
        printf("pattern \"%s\":\n", item->pattern);
        if (item->commands)
            pp_node(item->commands, indent + 2);
    }
    print_indent(indent);
    fputs("}\n", stdout);
}

/* Print simple constructs */
static void pp_simple(const struct ast_node *node, int indent)
{
    if (node->type == AST_SIMPLE_COMMAND)
        pp_simple_command(node->data, indent);
    else if (node->type == AST_LIST || node->type == AST_COMPOUND_LIST)
        pp_list(node->data, indent);
    else if (node->type == AST_IF)
        pp_if(node->data, indent);
    else if (node->type == AST_REDIRECTION)
        pp_redir(node->data, indent);
    else if (node->type == AST_PIPELINE)
        pp_pipeline(node->data, indent);
    else if (node->type == AST_NEGATION)
        pp_negation(node->data, indent);
}

/* Print complex constructs */
static void pp_complex(const struct ast_node *node, int indent)
{
    if (node->type == AST_WHILE)
        pp_loop(node->data, indent, "while");
    else if (node->type == AST_UNTIL)
        pp_loop(node->data, indent, "until");
    else if (node->type == AST_AND_OR)
        pp_and_or(node->data, indent);
    else if (node->type == AST_FOR)
        pp_for(node->data, indent);
    else if (node->type == AST_ASSIGNMENT)
        pp_assignment(node->data, indent);
    else if (node->type == AST_COMMAND_BLOCK)
    {
        struct command_block *b = node->data;
        print_indent(indent);
        fputs("command_block {\n", stdout);
        pp_node(b->commands, indent + 1);
        print_indent(indent);
        fputs("}\n", stdout);
    }
}

/* Print advanced constructs */
static void pp_advanced(const struct ast_node *node, int indent)
{
    if (node->type == AST_FUNCTION_DEF)
        pp_function_def(node->data, indent);
    else if (node->type == AST_SUBSHELL)
        pp_subshell(node->data, indent);
    else if (node->type == AST_BRACE_GROUP)
        pp_brace_group(node->data, indent);
    else if (node->type == AST_CASE)
        pp_case(node->data, indent);
}

/* Pretty print AST node */
static void pp_node(const struct ast_node *node, int indent)
{
    if (!node)
    {
        print_indent(indent);
        fputs("(null)\n", stdout);
        return;
    }

    pp_simple(node, indent);
    pp_complex(node, indent);
    pp_advanced(node, indent);
}

/*
 * Handle pretty print
 * @param const struct ast_node *node, int indent
 * @return void
 */

void pretty_print(const struct ast_node *node, int indent)
{
    pp_node(node, indent);
}
