#!/bin/bash
# Test: for three nested
for i in 1 2; do for j in a b; do for k in x y; do echo $i$j$k; done; done; done
