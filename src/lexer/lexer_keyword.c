#define _POSIX_C_SOURCE 200809L
#include <string.h>

#include "lexer.h"

/*
 * Get keyword type
 * @param const char *word
 * @return enum token_type
 */

enum token_type get_keyword_type(const char *word)
{
    if (strcmp(word, "if") == 0)
        return TOKEN_IF;
    else if (strcmp(word, "then") == 0)
        return TOKEN_THEN;
    else if (strcmp(word, "elif") == 0)
        return TOKEN_ELIF;
    else if (strcmp(word, "else") == 0)
        return TOKEN_ELSE;
    else if (strcmp(word, "fi") == 0)
        return TOKEN_FI;
    else if (strcmp(word, "while") == 0)
        return TOKEN_WHILE;
    else if (strcmp(word, "until") == 0)
        return TOKEN_UNTIL;
    else if (strcmp(word, "do") == 0)
        return TOKEN_DO;
    else if (strcmp(word, "done") == 0)
        return TOKEN_DONE;
    else if (strcmp(word, "for") == 0)
        return TOKEN_FOR;
    else if (strcmp(word, "in") == 0)
        return TOKEN_IN;
    else if (strcmp(word, "case") == 0)
        return TOKEN_CASE;
    else if (strcmp(word, "esac") == 0)
        return TOKEN_ESAC;

    return TOKEN_WORD;
}
