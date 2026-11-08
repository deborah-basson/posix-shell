#!/bin/bash
# Test: pipe redirect combo
echo test | cat >piped.txt; cat piped.txt; rm piped.txt
