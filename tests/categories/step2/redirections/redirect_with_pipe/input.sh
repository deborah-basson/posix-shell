#!/bin/bash
# Test: redirect with pipe
echo test | cat >piped.txt; cat piped.txt; rm piped.txt
