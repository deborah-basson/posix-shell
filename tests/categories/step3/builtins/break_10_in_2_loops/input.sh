#!/bin/bash
# Test: break 10 in 2 loops
for i in 1; do for j in 1; do break 10; echo no; done; echo no2; done; echo yes
