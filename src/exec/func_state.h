#ifndef FUNC_STATE_H
#define FUNC_STATE_H

struct func_state
{
    char **argv;
    int in_function;
};

extern struct func_state g_func_state;

#endif /* !FUNC_STATE_H */
