#!/bin/bash
# Test: until with redirect
i=0; until [ $i -eq 3 ]; do echo $i; i=$((i+1)); done >until.txt; cat until.txt; rm until.txt
