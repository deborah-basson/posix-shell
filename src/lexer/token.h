#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
    TOKEN_WORD,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELIF,
    TOKEN_ELSE,
    TOKEN_FI,
    TOKEN_SEMICOLON,
    TOKEN_NEWLINE,
    TOKEN_EOF,
    TOKEN_ERROR,

    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_REDIR_DUP_OUT,
    TOKEN_REDIR_DUP_IN,
    TOKEN_REDIR_CLOBBER,
    TOKEN_REDIR_RDWR,
    TOKEN_IONUMBER,

    TOKEN_PIPE,
    TOKEN_BANG,
    TOKEN_AND_IF,
    TOKEN_OR_IF,

    TOKEN_WHILE,
    TOKEN_UNTIL,
    TOKEN_DO,
    TOKEN_DONE,

    TOKEN_FOR,
    TOKEN_IN,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_CASE,
    TOKEN_ESAC,
    TOKEN_DSEMI

};

struct token
{
    enum token_type type;
    char *value;
    int io_number;
};

struct token *token_new(enum token_type type, const char *value);
void token_free(struct token *token);

int token_is_redirection(enum token_type type);

#endif /* !TOKEN_H */
