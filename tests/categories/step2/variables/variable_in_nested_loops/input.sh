#!/bin/bash
# Test: variable in nested loops
x=a; for i in 1 2; do for j in b c; do echo $x$i$j; done; done
