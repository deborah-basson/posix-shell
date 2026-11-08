#include <criterion/criterion.h>
#include <string.h>

#include "utils/utils.h"

Test(utils, str_dup_normal)
{
    char *result = str_dup("test string");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "test string");
    free(result);
}

Test(utils, str_dup_empty)
{
    char *result = str_dup("");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "");
    free(result);
}

Test(utils, str_dup_long)
{
    char *long_str = "This is a very long string to test str_dup";
    char *result = str_dup(long_str);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, long_str);
    free(result);
}

Test(utils, xmalloc_normal)
{
    void *ptr = xmalloc(100);
    cr_assert_not_null(ptr);
    free(ptr);
}

Test(utils, xmalloc_large)
{
    void *ptr = xmalloc(10000);
    cr_assert_not_null(ptr);
    free(ptr);
}

Test(utils, xrealloc_normal)
{
    void *ptr = xmalloc(10);
    cr_assert_not_null(ptr);

    void *new_ptr = xrealloc(ptr, 100);
    cr_assert_not_null(new_ptr);
    free(new_ptr);
}

Test(utils, xrealloc_null)
{
    void *ptr = xrealloc(NULL, 50);
    cr_assert_not_null(ptr);
    free(ptr);
}

Test(utils, xrealloc_shrink)
{
    void *ptr = xmalloc(1000);
    cr_assert_not_null(ptr);

    void *new_ptr = xrealloc(ptr, 10);
    cr_assert_not_null(new_ptr);
    free(new_ptr);
}
