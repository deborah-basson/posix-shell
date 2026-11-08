#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

#include "token.h"

struct lexer
{
    const char *input;
    size_t pos;
    size_t len;
    struct token *current;
    int owns_input;
};

struct lexer *lexer_new(const char *input);
void lexer_free(struct lexer *lexer);
struct token *lexer_peek(struct lexer *lexer);
struct token *lexer_next(struct lexer *lexer);

void skip_whitespace(struct lexer *lexer);
void skip_comment(struct lexer *lexer);

char *read_word(struct lexer *lexer);
char *read_single_quoted(struct lexer *lexer);
char *read_double_quoted_string(struct lexer *lexer);

char *handle_var_brace(struct lexer *lexer, char *word, size_t *len,
                       size_t *capacity);
char *handle_command_sub(struct lexer *lexer, char *word, size_t *len,
                         size_t *capacity);
char *handle_escape(struct lexer *lexer, char *word, size_t *len,
                    size_t *capacity);

char *ensure_capacity_quote(char *str, size_t *capacity, size_t len);
char *read_until_quote(struct lexer *lexer, char *str, size_t *len,
                       size_t *capacity);
char *handle_escaped_quote(struct lexer *lexer, char *str, size_t *len,
                           size_t *capacity);

char *handle_dq_escape(struct lexer *lexer, char *str, size_t *len,
                       size_t *capacity);
char *append_char_dq(char *str, size_t *len, size_t *capacity, char c);

struct token *lex_redirection(struct lexer *lexer);
struct token *lex_semicolon(struct lexer *lexer);
struct token *lex_newline(struct lexer *lexer);
struct token *lex_brace(struct lexer *lexer, char c);
struct token *lex_pipe(struct lexer *lexer);
struct token *lex_ampersand(struct lexer *lexer);
struct token *lex_bang(struct lexer *lexer);
struct token *lex_paren(struct lexer *lexer, char c);
struct token *lex_ionumber(struct lexer *lexer);
struct token *lex_token(struct lexer *lexer);

enum token_type get_keyword_type(const char *word);

#endif /* !LEXER_H */
