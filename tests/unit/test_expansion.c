#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

#include "expansion/expansion.h"
#include "variables/variables.h"

void setup_vars(void)
{
    init_global_vars();
}

void teardown_vars(void)
{
    cleanup_global_vars();
}

TestSuite(expansion, .init = setup_vars, .fini = teardown_vars);

Test(expansion, expand_word_simple)
{
    char *result = expand_word("hello");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "hello");
    free(result);
}

Test(expansion, expand_word_with_variable)
{
    var_set(g_var_table, "TEST", "world");
    char *result = expand_word("hello $TEST");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "hello world");
    free(result);
    var_unset(g_var_table, "TEST");
}

Test(expansion, expand_word_with_braces)
{
    var_set(g_var_table, "VAR", "test");
    char *result = expand_word("${VAR}");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "test");
    free(result);
    var_unset(g_var_table, "VAR");
}

Test(expansion, expand_word_empty_variable)
{
    var_unset(g_var_table, "NOTSET");
    char *result = expand_word("$NOTSET");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "");
    free(result);
}

Test(expansion, expand_word_multiple_variables)
{
    var_set(g_var_table, "A", "foo");
    var_set(g_var_table, "B", "bar");
    char *result = expand_word("$A-$B");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "foo-bar");
    free(result);
    var_unset(g_var_table, "A");
    var_unset(g_var_table, "B");
}

Test(expansion, expand_word_special_var_question_mark)
{
    char *result = expand_word("$?");
    cr_assert_not_null(result);
    free(result);
}

Test(expansion, expand_word_special_var_dollar_dollar)
{
    char *result = expand_word("$$");
    cr_assert_not_null(result);
    free(result);
}

Test(expansion, expand_word_literal_dollar)
{
    char *result = expand_word("\\$");
    cr_assert_not_null(result);
    free(result);
}
