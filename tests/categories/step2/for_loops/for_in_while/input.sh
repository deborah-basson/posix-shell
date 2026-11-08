#!/bin/bash
# Test: for in while
j=0; while [ $j -lt 2 ]; do for i in a b; do echo $j$i; done; j=$((j+1)); done
