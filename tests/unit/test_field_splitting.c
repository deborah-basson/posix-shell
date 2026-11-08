#include <criterion/criterion.h>
#include <string.h>

#include "expansion/field_splitting.h"
#include "variables/variables.h"

void setup_fs(void)
{
    init_global_vars();
}

void teardown_fs(void)
{
    cleanup_global_vars();
}

TestSuite(field_splitting, .init = setup_fs, .fini = teardown_fs);

Test(field_splitting, split_simple_spaces)
{
    const char *input = "a b c";
    const char *ifs = " \t\n";
    char **result = field_split(input, ifs);

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "b");
    cr_assert_str_eq(result[2], "c");
    cr_assert_null(result[3]);

    free_split_result(result);
}

Test(field_splitting, split_multiple_spaces)
{
    const char *input = "a  b    c";
    const char *ifs = " \t\n";
    char **result = field_split(input, ifs);

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "b");
    cr_assert_str_eq(result[2], "c");
    cr_assert_null(result[3]);

    free_split_result(result);
}

Test(field_splitting, split_custom_ifs)
{
    const char *input = "a:b:c";
    const char *ifs = ":";
    char **result = field_split(input, ifs);

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "b");
    cr_assert_str_eq(result[2], "c");
    cr_assert_null(result[3]);

    free_split_result(result);
}

Test(field_splitting, split_empty_fields)
{
    const char *input = "a::c";
    const char *ifs = ":";
    char **result = field_split(input, ifs);

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "");
    cr_assert_str_eq(result[2], "c");
    cr_assert_null(result[3]);

    free_split_result(result);
}

Test(field_splitting, split_no_ifs)
{
    const char *input = "abc";
    const char *ifs = "";
    char **result = field_split(input, ifs);

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "abc");
    cr_assert_null(result[1]);

    free_split_result(result);
}

Test(field_splitting, split_single_word)
{
    const char *input = "hello";
    const char *ifs = " \t\n";
    char **result = field_split(input, ifs);

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "hello");
    cr_assert_null(result[1]);

    free_split_result(result);
}

Test(field_splitting, split_leading_trailing_spaces)
{
    const char *input = "  a b c  ";
    const char *ifs = " \t\n";
    char **result = field_split(input, ifs);

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "b");
    cr_assert_str_eq(result[2], "c");
    cr_assert_null(result[3]);

    free_split_result(result);
}
