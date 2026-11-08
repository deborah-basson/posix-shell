#include <criterion/criterion.h>
#include <string.h>

#include "lexer/lexer.h"
#include "lexer/token.h"

Test(lexer, lex_simple_word)
{
    struct lexer *lex = lexer_new("hello");
    cr_assert_not_null(lex);

    struct token *tok = lexer_peek(lex);
    cr_assert_not_null(tok);
    cr_assert_eq(tok->type, TOKEN_WORD);
    cr_assert_str_eq(tok->value, "hello");

    lexer_free(lex);
}

Test(lexer, lex_multiple_words)
{
    struct lexer *lex = lexer_new("echo hello world");
    cr_assert_not_null(lex);

    struct token *tok1 = lexer_next(lex);
    cr_assert_not_null(tok1);
    cr_assert_eq(tok1->type, TOKEN_WORD);
    cr_assert_str_eq(tok1->value, "echo");

    struct token *tok2 = lexer_next(lex);
    cr_assert_not_null(tok2);
    cr_assert_eq(tok2->type, TOKEN_WORD);
    cr_assert_str_eq(tok2->value, "hello");

    struct token *tok3 = lexer_next(lex);
    cr_assert_not_null(tok3);
    cr_assert_eq(tok3->type, TOKEN_WORD);
    cr_assert_str_eq(tok3->value, "world");

    lexer_free(lex);
}

Test(lexer, lex_semicolon)
{
    struct lexer *lex = lexer_new("echo hello;");
    cr_assert_not_null(lex);

    lexer_next(lex); // echo
    lexer_next(lex); // hello

    struct token *tok = lexer_next(lex);
    cr_assert_not_null(tok);
    cr_assert_eq(tok->type, TOKEN_SEMICOLON);

    lexer_free(lex);
}

Test(lexer, lex_newline)
{
    struct lexer *lex = lexer_new("echo hello\n");
    cr_assert_not_null(lex);

    lexer_next(lex); // echo
    lexer_next(lex); // hello

    struct token *tok = lexer_next(lex);
    cr_assert_not_null(tok);
    cr_assert_eq(tok->type, TOKEN_NEWLINE);

    lexer_free(lex);
}

Test(lexer, lex_pipe)
{
    struct lexer *lex = lexer_new("echo hello | cat");
    cr_assert_not_null(lex);

    lexer_next(lex); // echo
    lexer_next(lex); // hello

    struct token *tok = lexer_next(lex);
    cr_assert_not_null(tok);
    cr_assert_eq(tok->type, TOKEN_PIPE);

    lexer_free(lex);
}

Test(lexer, lex_redirect_output)
{
    struct lexer *lex = lexer_new("echo hello > file");
    cr_assert_not_null(lex);

    lexer_next(lex); // echo
    lexer_next(lex); // hello

    struct token *tok = lexer_next(lex);
    cr_assert_not_null(tok);
    cr_assert_eq(tok->type, TOKEN_REDIR_OUT);

    lexer_free(lex);
}

Test(lexer, lex_redirect_input)
{
    struct lexer *lex = lexer_new("cat < file");
    cr_assert_not_null(lex);

    lexer_next(lex); // cat

    struct token *tok = lexer_next(lex);
    cr_assert_not_null(tok);
    cr_assert_eq(tok->type, TOKEN_REDIR_IN);

    lexer_free(lex);
}

Test(lexer, lex_if_keyword)
{
    struct lexer *lex = lexer_new("if true; then");
    cr_assert_not_null(lex);

    struct token *tok = lexer_next(lex);
    cr_assert_not_null(tok);
    cr_assert_eq(tok->type, TOKEN_IF);

    lexer_free(lex);
}

Test(lexer, lex_eof)
{
    struct lexer *lex = lexer_new("");
    cr_assert_not_null(lex);

    struct token *tok = lexer_next(lex);
    cr_assert_not_null(tok);
    cr_assert_eq(tok->type, TOKEN_EOF);

    lexer_free(lex);
}

Test(lexer, lex_and_operator)
{
    struct lexer *lex = lexer_new("true && false");
    cr_assert_not_null(lex);

    lexer_next(lex); // true

    struct token *tok = lexer_next(lex);
    cr_assert_not_null(tok);
    cr_assert_eq(tok->type, TOKEN_AND_IF);

    lexer_free(lex);
}

Test(lexer, lex_or_operator)
{
    struct lexer *lex = lexer_new("true || false");
    cr_assert_not_null(lex);

    lexer_next(lex); // true

    struct token *tok = lexer_next(lex);
    cr_assert_not_null(tok);
    cr_assert_eq(tok->type, TOKEN_OR_IF);

    lexer_free(lex);
}
