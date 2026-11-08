#ifndef EXPANSION_H
#define EXPANSION_H

#include <stddef.h>

char *expand_word(const char *word);

char *parse_var_name(const char *word, size_t *pos, int *use_braces);
char *expand_special_var(const char *var_name);
char *expand_argc_var(void);
char *expand_all_args_var(void);
char *expand_positional_arg(const char *var_name);
char *expand_variable_at_pos(const char *word, size_t *pos);

char *ensure_capacity_expand(char *result, size_t *capacity, size_t needed);
char *append_expansion(char *result, size_t *result_len, size_t *capacity,
                       char *expansion);
char *append_char_expand(char *result, size_t *result_len, size_t *capacity,
                         char c);

char *expand_arithmetic(const char *word, size_t *pos);
char *expand_command_substitution(const char *word, size_t *pos);

char **expand_argv(char **argv, int argc);
void free_expanded_argv(char **argv, int count);

#endif /* !EXPANSION_H */
