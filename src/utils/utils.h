#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

char *str_dup(const char *s);
void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);

#endif /* !UTILS_H */
