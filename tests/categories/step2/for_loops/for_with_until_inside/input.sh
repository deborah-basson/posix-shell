#!/bin/bash
# Test: for with until inside
for i in 1 2; do j=0; until [ $j -eq 2 ]; do echo $i$j; j=$((j+1)); done; done
