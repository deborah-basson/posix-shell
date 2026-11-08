noinst_PROGRAMS = \
    test_expansion \
    test_field_splitting \
    test_variables \
    test_lexer \
    test_pattern_match \
    test_utils

TESTS = \
    test_expansion \
    test_field_splitting \
    test_variables \
    test_lexer \
    test_pattern_match \
    test_utils

AM_CFLAGS = -std=c99 -pedantic -Wall -Wextra -Wvla -I$(top_srcdir)/src

LDADD = \
    $(top_builddir)/src/utils/libutils.a \
    $(top_builddir)/src/variables/libvariables.a \
    $(top_builddir)/src/builtins/libbuiltins.a \
    $(top_builddir)/src/ast/libast.a \
    $(top_builddir)/src/parser/libparser.a \
    $(top_builddir)/src/lexer/liblexer.a \
    $(top_builddir)/src/exec/libexec.a \
    $(top_builddir)/src/expansion/libexpansion.a \
    $(top_builddir)/src/exec/libexec.a \
    $(top_builddir)/src/expansion/libexpansion.a \
    $(top_builddir)/src/pretty_print/libpretty_print.a \
    -lcriterion

test_expansion_SOURCES = test_expansion.c test_stubs_full.c
test_expansion_CFLAGS = $(AM_CFLAGS)
test_expansion_LDADD = $(LDADD)

test_field_splitting_SOURCES = test_field_splitting.c test_stubs_full.c
test_field_splitting_CFLAGS = $(AM_CFLAGS)
test_field_splitting_LDADD = $(LDADD)

test_variables_SOURCES = test_variables.c test_stubs_full.c
test_variables_CFLAGS = $(AM_CFLAGS)
test_variables_LDADD = $(LDADD)

test_lexer_SOURCES = test_lexer.c test_stubs_lexer.c
test_lexer_CFLAGS = $(AM_CFLAGS)
test_lexer_LDADD = $(LDADD)

test_pattern_match_SOURCES = test_pattern_match.c test_stubs_full.c
test_pattern_match_CFLAGS = $(AM_CFLAGS)
test_pattern_match_LDADD = $(LDADD)

test_utils_SOURCES = test_utils.c test_stubs_full.c
test_utils_CFLAGS = $(AM_CFLAGS)
test_utils_LDADD = $(LDADD)
