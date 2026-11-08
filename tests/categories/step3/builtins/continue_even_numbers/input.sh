#!/bin/bash
# Test: continue even numbers
for i in 1 2 3 4 5 6 7 8 9 10; do if [ $((i % 2)) -eq 0 ]; then continue; fi; echo $i; done
