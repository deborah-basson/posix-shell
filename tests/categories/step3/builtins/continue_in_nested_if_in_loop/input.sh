#!/bin/bash
# Test: continue in nested if in loop
for i in 1 2 3 4 5; do if true; then if [ $i -eq 3 ]; then continue; fi; fi; echo $i; done
