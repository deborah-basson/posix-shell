#!/bin/bash
# Test: redirect append ten lines
echo 1 >f.txt; echo 2 >>f.txt; echo 3 >>f.txt; echo 4 >>f.txt; echo 5 >>f.txt; echo 6 >>f.txt; echo 7 >>f.txt; echo 8 >>f.txt; echo 9 >>f.txt; echo 10 >>f.txt; cat f.txt; rm f.txt
