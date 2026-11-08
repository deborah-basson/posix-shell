#include "error.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Handle error
 * @param const char *msg
 * @return void
 */

void error(const char *msg)
{
    fprintf(stderr, "posix-shell: %s\n", msg);
}

/*
 * Handle error exit
 * @param const char *msg, int status
 * @return void
 */

void error_exit(const char *msg, int status)
{
    error(msg);
    exit(status);
}
