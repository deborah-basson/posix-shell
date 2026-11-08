#!/bin/bash
# Test: while with for inside
i=0; while [ $i -lt 2 ]; do for j in a b; do echo $i$j; done; i=$((i+1)); done
