#!/bin/bash
# Test: append multiple times
echo a >f.txt; echo b >>f.txt; echo c >>f.txt; cat f.txt; rm f.txt
