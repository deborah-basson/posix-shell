#include <criterion/criterion.h>

#include "expansion/pattern_match.h"

Test(pattern_match, exact_match)
{
    int result = pattern_match("hello", "hello");
    cr_assert(result);
}

Test(pattern_match, no_match)
{
    int result = pattern_match("hello", "world");
    cr_assert_not(result);
}

Test(pattern_match, star_matches_all)
{
    int result = pattern_match("*", "anything");
    cr_assert(result);
}

Test(pattern_match, star_prefix)
{
    int result = pattern_match("*.txt", "file.txt");
    cr_assert(result);
}

Test(pattern_match, star_suffix)
{
    int result = pattern_match("file*", "file.txt");
    cr_assert(result);
}

Test(pattern_match, star_middle)
{
    int result = pattern_match("f*e", "file");
    cr_assert(result);
}

Test(pattern_match, question_mark_single_char)
{
    int result = pattern_match("hel?o", "hello");
    cr_assert(result);
}

Test(pattern_match, question_mark_no_match)
{
    int result = pattern_match("hel?o", "helo");
    cr_assert_not(result);
}

Test(pattern_match, brackets_range)
{
    int result = pattern_match("[a-z]", "m");
    cr_assert(result);
}

Test(pattern_match, brackets_no_match)
{
    int result = pattern_match("[a-z]", "5");
    cr_assert_not(result);
}

Test(pattern_match, brackets_negation)
{
    int result = pattern_match("[!0-9]", "a");
    cr_assert(result);
}

Test(pattern_match, complex_pattern)
{
    int result = pattern_match("file_[0-9]*.txt", "file_42_backup.txt");
    cr_assert(result);
}

Test(pattern_match, empty_pattern)
{
    int result = pattern_match("", "");
    cr_assert(result);
}

Test(pattern_match, empty_string_no_match)
{
    int result = pattern_match("test", "");
    cr_assert_not(result);
}

Test(pattern_match, multiple_stars)
{
    int result = pattern_match("*test*", "mytestfile");
    cr_assert(result);
}

Test(pattern_match, star_at_end)
{
    int result = pattern_match("test*", "testing");
    cr_assert(result);
}
