#include "pattern_match.h"

#include <string.h>

/* Helper for match bracket range */
static int match_bracket_range(const char **pattern, char c)
{
    int matched = 0;
    while (**pattern && **pattern != ']')
    {
        if ((*pattern)[1] == '-' && (*pattern)[2] != ']')
        {
            if (c >= (*pattern)[0] && c <= (*pattern)[2])
                matched = 1;
            *pattern += 3;
        }
        else
        {
            if (**pattern == c)
                matched = 1;
            (*pattern)++;
        }
    }
    return matched;
}

/* Helper for match bracket */
static int match_bracket(const char *pattern, char c, const char **next_pattern)
{
    pattern++;
    int negate = 0;
    if (*pattern == '!')
    {
        negate = 1;
        pattern++;
    }

    int matched = match_bracket_range(&pattern, c);

    if (*pattern == ']')
        pattern++;

    *next_pattern = pattern;
    return negate ? !matched : matched;
}

/* Helper for match char */
static int match_char(const char *pattern, char c, const char **next_pattern)
{
    if (*pattern == '[')
        /*
         * Handle match bracket
         * @param pattern, c, next_pattern
         * @return return
         */

        return match_bracket(pattern, c, next_pattern);

    if (*pattern == '?')
    {
        *next_pattern = pattern + 1;
        return 1;
    }

    if (*pattern == c)
    {
        *next_pattern = pattern + 1;
        return 1;
    }

    return 0;
}

/* Helper for match wildcard */
static int match_wildcard(const char *pattern, const char *string)
{
    pattern++;
    if (*pattern == '\0')
        return 1;

    for (const char *s = string; *s; s++)
    {
        if (pattern_match(pattern, s))
            return 1;
    }
    /*
     * Handle pattern match
     * @param pattern, string + strlen(string
     * @return return
     */

    return pattern_match(pattern, string + strlen(string));
}

/*
 * Handle pattern match
 * @param const char *pattern, const char *string
 * @return int
 */

int pattern_match(const char *pattern, const char *string)
{
    if (*pattern == '\0' && *string == '\0')
        return 1;

    if (*pattern == '\0')
        return 0;

    if (*pattern == '*')
        return match_wildcard(pattern, string);

    if (*string == '\0')
        return 0;

    const char *next_pattern;
    if (match_char(pattern, *string, &next_pattern))
        return pattern_match(next_pattern, string + 1);

    return 0;
}
