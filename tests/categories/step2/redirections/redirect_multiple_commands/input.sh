#!/bin/bash
# Test: redirect multiple commands
echo a >f.txt; echo b >>f.txt; echo c >>f.txt; echo d >>f.txt; echo e >>f.txt; cat f.txt; rm f.txt
