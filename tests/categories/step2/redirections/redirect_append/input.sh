#!/bin/bash
# Test: redirect append
echo a >f.txt; echo b >>f.txt; cat f.txt; rm f.txt
