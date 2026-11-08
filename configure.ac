AC_PREREQ([2.69])
AC_INIT([posix-shell], [1.0])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall])
AC_PROG_CC
AC_PROG_AR
AC_PROG_RANLIB
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
    src/lexer/Makefile
    src/parser/Makefile
    src/ast/Makefile
    src/expansion/Makefile
    src/exec/Makefile
    src/builtins/Makefile
    src/utils/Makefile
    src/variables/Makefile
    src/pretty_print/Makefile
    tests/Makefile
    tests/unit/Makefile
])
AC_OUTPUT
