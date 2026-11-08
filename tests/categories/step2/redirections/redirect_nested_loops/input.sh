#!/bin/bash
# Test: redirect nested loops
for i in 1 2; do for j in a b; do echo $i$j; done; done >nested.txt; cat nested.txt; rm nested.txt
