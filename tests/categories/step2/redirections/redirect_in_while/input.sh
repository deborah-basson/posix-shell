#!/bin/bash
# Test: redirect in while
i=0; while [ $i -lt 2 ]; do echo $i >>loop.txt; i=$((i+1)); done; cat loop.txt; rm loop.txt
