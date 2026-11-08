#!/bin/bash
# Test: redirect twenty lines
echo 1 >f.txt; echo 2 >>f.txt; echo 3 >>f.txt; echo 4 >>f.txt; echo 5 >>f.txt; echo 6 >>f.txt; echo 7 >>f.txt; echo 8 >>f.txt; echo 9 >>f.txt; echo 10 >>f.txt; echo 11 >>f.txt; echo 12 >>f.txt; echo 13 >>f.txt; echo 14 >>f.txt; echo 15 >>f.txt; echo 16 >>f.txt; echo 17 >>f.txt; echo 18 >>f.txt; echo 19 >>f.txt; echo 20 >>f.txt; cat f.txt; rm f.txt
