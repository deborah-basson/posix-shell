#!/bin/bash
# Test: while with redirect
i=0; while [ $i -lt 3 ]; do echo $i; i=$((i+1)); done >while.txt; cat while.txt; rm while.txt
