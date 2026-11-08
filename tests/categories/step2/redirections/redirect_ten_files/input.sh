#!/bin/bash
# Test: redirect ten files
echo 1 >f1.txt; echo 2 >f2.txt; echo 3 >f3.txt; echo 4 >f4.txt; echo 5 >f5.txt; echo 6 >f6.txt; echo 7 >f7.txt; echo 8 >f8.txt; echo 9 >f9.txt; echo 10 >f10.txt; cat f1.txt f2.txt f3.txt f4.txt f5.txt f6.txt f7.txt f8.txt f9.txt f10.txt; rm f*.txt
