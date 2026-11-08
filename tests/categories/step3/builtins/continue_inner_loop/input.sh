#!/bin/bash
# Test: continue inner loop
for i in 1 2; do for j in a b c; do if [ $j = b ]; then continue; fi; echo $i$j; done; done
