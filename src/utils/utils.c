#define _POSIX_C_SOURCE 200809L
#include "utils.h"

#include <stdlib.h>
#include <string.h>

char *str_dup(const char *s)
{
    if (!s)
        return NULL;
    /*
     * Handle strdup
     * @param s
     * @return return
     */

    return strdup(s);
}

void *xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr && size != 0)
        exit(1);
    return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size);
    if (!new_ptr && size != 0)
        exit(1);
    return new_ptr;
}
