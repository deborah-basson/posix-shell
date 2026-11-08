SUBDIRS = src tests

AM_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

ACLOCAL_AMFLAGS = -I m4

DISTCLEANFILES = config.log config.status
MAINTAINERCLEANFILES = \
	Makefile.in \
	aclocal.m4 \
	configure \
	config.h.in

maintainer-clean-local:
	rm -rf autom4te.cache build-aux m4
