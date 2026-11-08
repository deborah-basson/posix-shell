#ifndef FIELD_SPLITTING_H
#define FIELD_SPLITTING_H

/*
** Quote markers used to track quoted regions during expansion.
** Single quotes prevent all expansion, double quotes prevent field splitting.
*/
#define SQUOTE_START '\x02'
#define SQUOTE_END '\x03'
#define DQUOTE_START '\x04'
#define DQUOTE_END '\x05'

/*
** Get current IFS value from environment.
** Returns default " \t\n" if IFS is unset, caller must free.
*/
char *get_ifs(void);

/*
** Split input string into fields according to IFS rules.
** Returns NULL-terminated array of strings, caller must free with
*free_split_result.
*/
char **field_split(const char *input, const char *ifs);

void free_split_result(char **result);
int is_ifs_whitespace(char c, const char *ifs);

#endif /* !FIELD_SPLITTING_H */
