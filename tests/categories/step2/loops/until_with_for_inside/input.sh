#!/bin/bash
# Test: until with for inside
i=0; until [ $i -eq 2 ]; do for j in a b; do echo $i$j; done; i=$((i+1)); done
