#!/bin/sh

make maintainer-clean 2>/dev/null || true
make distclean 2>/dev/null || true
make clean 2>/dev/null || true

rm -rf autom4te.cache/
rm -rf build-aux/
rm -f aclocal.m4
rm -f config.h.in
rm -f config.log
rm -f config.status
rm -f configure
rm -f stamp-h1
rm -f Makefile
rm -f Makefile.in

find src -name "Makefile" -delete
find src -name "Makefile.in" -delete
find src -name "*.o" -delete
find src -name "*.a" -delete
find src -name ".deps" -type d -exec rm -rf {} + 2>/dev/null
find tests/unit -name ".deps" -type d -exec rm -rf {} + 2>/dev/null

find tests -name "Makefile" -delete
find tests -name "Makefile.in" -delete

rm -f '$(echo file.txt)'
rm -f tests/'$(echo file.txt)'
rm -f *.txt
rm -f tests/*.txt
rm -f tests/unit/*.o
rm -f 'configure~'

rm -f src/posix-shell

echo "✓ Project cleaned"
