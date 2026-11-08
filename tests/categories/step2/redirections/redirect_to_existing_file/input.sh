#!/bin/bash
# Test: redirect to existing file
echo old >f.txt; echo new >f.txt; cat f.txt; rm f.txt
