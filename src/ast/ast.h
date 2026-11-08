#ifndef AST_H
#define AST_H

#include "../lexer/token.h"

enum ast_node_type
{
    AST_SIMPLE_COMMAND,
    AST_LIST,
    AST_IF,
    AST_COMPOUND_LIST,
    AST_REDIRECTION,
    AST_PIPELINE,
    AST_NEGATION,
    AST_WHILE,
    AST_UNTIL,
    AST_AND_OR,
    AST_FOR,
    AST_ASSIGNMENT,
    AST_COMMAND_BLOCK,
    AST_FUNCTION_DEF,
    AST_SUBSHELL,
    AST_BRACE_GROUP,
    AST_CASE
};

struct case_item
{
    char *pattern;
    struct ast_node *commands;
    struct case_item *next;
};

struct case_node
{
    char *word;
    struct case_item *items;
};

struct ast_node
{
    enum ast_node_type type;
    void *data;
    int ref_count;
};

struct simple_command
{
    char **argv;
    int argc;
};

struct list_node
{
    struct ast_node **commands;
    int count;
};

struct elif_clause
{
    struct ast_node *condition;
    struct ast_node *then_part;
};

struct if_node
{
    struct ast_node *condition;
    struct ast_node *then_part;
    struct elif_clause *elif_parts;
    int elif_count;
    struct ast_node *else_part;
};

struct redirection
{
    enum token_type redir_type;
    int io_number;
    char *target;
};

struct command_with_redirs
{
    struct ast_node *command;
    struct redirection **redirs;
    int redir_count;
};

struct pipeline_node
{
    struct ast_node **commands;
    int count;
};

struct negation_node
{
    struct ast_node *command;
};

struct loop_node
{
    struct ast_node *condition;
    struct ast_node *body;
};

struct and_or_node
{
    struct ast_node *left;
    struct ast_node *right;
    int is_and;
};

struct for_node
{
    char *variable;
    char **words;
    int word_count;
    struct ast_node *body;
};

struct assignment_node
{
    char *name;
    char *value;
};

struct command_block
{
    struct ast_node *commands;
};

struct function_def_node
{
    char *name;
    struct ast_node *body;
    struct redirection **redirs;
    int redir_count;
};

struct subshell_node
{
    struct ast_node *command;
};

struct brace_group_node
{
    struct ast_node *command;
};

struct ast_node *ast_node_new(enum ast_node_type type, void *data);
void ast_free(struct ast_node *node);
void ast_retain(struct ast_node *node);
void ast_release(struct ast_node *node);

struct simple_command *simple_command_new(char **argv, int argc);
void simple_command_free(struct simple_command *cmd);

struct list_node *list_node_new(void);
void list_node_add(struct list_node *list, struct ast_node *node);
void list_node_free(struct list_node *list);

struct if_node *if_node_new(void);
void if_node_free(struct if_node *node);

struct redirection *redirection_new(enum token_type type, int io_number,
                                    const char *target);
void redirection_free(struct redirection *redir);

struct command_with_redirs *command_with_redirs_new(void);
void command_with_redirs_add_redir(struct command_with_redirs *cmd,
                                   struct redirection *redir);
void command_with_redirs_free(struct command_with_redirs *cmd);

struct pipeline_node *pipeline_node_new(void);
void pipeline_node_add(struct pipeline_node *pipeline, struct ast_node *cmd);
void pipeline_node_free(struct pipeline_node *pipeline);

struct negation_node *negation_node_new(struct ast_node *cmd);
void negation_node_free(struct negation_node *neg);

struct loop_node *loop_node_new(struct ast_node *cond, struct ast_node *body);
void loop_node_free(struct loop_node *loop);

struct and_or_node *and_or_node_new(struct ast_node *left,
                                    struct ast_node *right, int is_and);
void and_or_node_free(struct and_or_node *node);

struct for_node *for_node_new(const char *var);
void for_node_add_word(struct for_node *node, const char *word);
void for_node_free(struct for_node *node);

struct assignment_node *assignment_node_new(const char *name,
                                            const char *value);
void assignment_node_free(struct assignment_node *node);

struct function_def_node *function_def_node_new(const char *name,
                                                struct ast_node *body);
void function_def_node_free(struct function_def_node *node);

struct subshell_node *subshell_node_new(struct ast_node *command);
void subshell_node_free(struct subshell_node *node);

struct brace_group_node *brace_group_node_new(struct ast_node *command);
void brace_group_node_free(struct brace_group_node *node);

#endif /* !AST_H */
