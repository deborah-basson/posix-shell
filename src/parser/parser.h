#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"

struct ast_node *parse(struct lexer *lexer);

int match(struct lexer *lexer, enum token_type type);
void skip_newlines(struct lexer *lexer);
struct ast_node *parse_and_or(struct lexer *lexer);

void parser_error(const char *msg);
int parser_had_error(void);
void parser_reset_error(void);

#endif /* !PARSER_H */
