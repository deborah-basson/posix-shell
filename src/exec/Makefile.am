noinst_LIBRARIES = libexec.a

libexec_a_SOURCES = \
    exec.c exec.h \
    simple_command.c simple_command.h \
    redirection.c redirection.h \
    pipeline.c pipeline.h \
    func_globals.c func_argv_globals.c \
    functions_part1.c functions_part2.c \
    functions.h \
    global_argc.h \
    global_in_function.h \
    global_state.c global_state.h \
    alias_table.c alias_table.h
libexec_a_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla -I$(top_srcdir)/src
