#!/bin/bash
# Test: break inner loop
for i in 1 2; do for j in a b c; do echo $i$j; if [ $j = b ]; then break; fi; done; done
