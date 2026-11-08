SUBDIRS = utils lexer ast parser builtins expansion exec variables pretty_print

bin_PROGRAMS = posix-shell

posix_shell_SOURCES = main.c

posix_shell_LDADD = \
    lexer/liblexer.a \
    parser/libparser.a \
    ast/libast.a \
    exec/libexec.a \
    expansion/libexpansion.a \
    builtins/libbuiltins.a \
    utils/libutils.a \
    variables/libvariables.a \
    pretty_print/libpretty_print.a

posix_shell_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
