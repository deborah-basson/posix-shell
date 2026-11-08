#!/bin/bash
# Test: for with while inside
for i in 1 2; do j=0; while [ $j -lt 2 ]; do echo $i$j; j=$((j+1)); done; done
