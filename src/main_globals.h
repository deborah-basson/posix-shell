#ifndef MAIN_GLOBALS_H
#define MAIN_GLOBALS_H

struct main_globals
{
    int argc;
    char **argv;
    int pretty_print;
};

extern struct main_globals g_main;

#endif /* !MAIN_GLOBALS_H */
