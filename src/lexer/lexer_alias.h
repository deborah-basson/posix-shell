#ifndef LEXER_ALIAS_H
#define LEXER_ALIAS_H

#include "lexer.h"

// Vérifie et substitue un alias si nécessaire
// Retourne 1 si un alias a été expansé, 0 sinon
int lexer_try_expand_alias(struct lexer *lexer, const char *word,
                           char **expanded_input, size_t *expanded_len);

#endif /* !LEXER_ALIAS_H */
