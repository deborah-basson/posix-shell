#!/bin/bash
# Test: until with while inside
i=0; until [ $i -eq 2 ]; do j=0; while [ $j -lt 2 ]; do echo $i$j; j=$((j+1)); done; i=$((i+1)); done
