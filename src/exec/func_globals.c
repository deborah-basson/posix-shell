#define _POSIX_C_SOURCE 200809L
#include <stddef.h>

#include "functions.h"

struct function_globals g_function_globals = { .table = NULL };
