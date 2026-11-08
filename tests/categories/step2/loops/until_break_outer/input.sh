#!/bin/bash
# Test: until break outer
i=0; until [ $i -eq 3 ]; do j=0; until [ $j -eq 3 ]; do if [ $i -eq 1 ] && [ $j -eq 1 ]; then break 2; fi; echo $i$j; j=$((j+1)); done; i=$((i+1)); done
