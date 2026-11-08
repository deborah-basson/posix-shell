#define _POSIX_C_SOURCE 200809L
#include <stddef.h>

#include "func_state.h"

struct func_state g_func_state = { .argv = NULL, .in_function = 0 };
