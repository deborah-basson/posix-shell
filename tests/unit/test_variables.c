// tests/unit/test_variables.c
#include <criterion/criterion.h>
#include <string.h>

#include "variables/variables.h"

void setup_var(void)
{
    init_global_vars();
}

void teardown_var(void)
{
    cleanup_global_vars();
}

TestSuite(variables, .init = setup_var, .fini = teardown_var);

Test(variables, set_and_get_variable)
{
    var_set(g_var_table, "TEST", "value");
    char *result = var_get(g_var_table, "TEST");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "value");
    var_unset(g_var_table, "TEST");
}

Test(variables, get_nonexistent_variable)
{
    var_unset(g_var_table, "NONEXISTENT");
    char *result = var_get(g_var_table, "NONEXISTENT");
    cr_assert_null(result);
}

Test(variables, overwrite_variable)
{
    var_set(g_var_table, "TEST", "old");
    var_set(g_var_table, "TEST", "new");
    char *result = var_get(g_var_table, "TEST");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "new");
    var_unset(g_var_table, "TEST");
}

Test(variables, unset_variable)
{
    var_set(g_var_table, "TEST", "value");
    var_unset(g_var_table, "TEST");
    char *result = var_get(g_var_table, "TEST");
    cr_assert_null(result);
}

Test(variables, set_empty_variable)
{
    var_set(g_var_table, "EMPTY", "");
    char *result = var_get(g_var_table, "EMPTY");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "");
    var_unset(g_var_table, "EMPTY");
}

Test(variables, multiple_variables)
{
    var_set(g_var_table, "VAR1", "value1");
    var_set(g_var_table, "VAR2", "value2");
    var_set(g_var_table, "VAR3", "value3");

    char *v1 = var_get(g_var_table, "VAR1");
    char *v2 = var_get(g_var_table, "VAR2");
    char *v3 = var_get(g_var_table, "VAR3");

    cr_assert_str_eq(v1, "value1");
    cr_assert_str_eq(v2, "value2");
    cr_assert_str_eq(v3, "value3");

    var_unset(g_var_table, "VAR1");
    var_unset(g_var_table, "VAR2");
    var_unset(g_var_table, "VAR3");
}

Test(variables, variable_with_special_chars)
{
    var_set(g_var_table, "TEST", "hello world!@#$%");
    char *result = var_get(g_var_table, "TEST");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "hello world!@#$%");
    var_unset(g_var_table, "TEST");
}