#!/bin/bash
# Test: redirect noclobber
echo a >f.txt; echo b >|f.txt; cat f.txt; rm f.txt
