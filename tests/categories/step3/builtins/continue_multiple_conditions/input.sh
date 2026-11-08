#!/bin/bash
# Test: continue multiple conditions
for i in 1 2 3 4 5 6 7 8 9 10; do if [ $i -eq 2 ] || [ $i -eq 5 ] || [ $i -eq 8 ]; then continue; fi; echo $i; done
