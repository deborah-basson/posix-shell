#!/bin/bash
# Test: for break outer
for i in 1 2 3; do for j in a b c; do if [ $i -eq 2 ] && [ $j = b ]; then break 2; fi; echo $i$j; done; done
