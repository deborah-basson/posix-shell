noinst_LIBRARIES = libexpansion.a

libexpansion_a_SOURCES = \
    expansion.c expansion.h \
    expansion_argv.c \
    expansion_var_helpers.c \
    expansion_var_all.c \
    expansion_var_positional.c \
    expansion_var_parse.c \
    expansion_var_main.c \
    expansion_word_helpers.c \
    expansion_word_main.c \
    field_splitting.c field_splitting.h \
    pattern_match.c pattern_match.h

libexpansion_a_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla -I$(top_srcdir)/src
