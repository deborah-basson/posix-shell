#!/bin/bash
# Test: break 2 levels
for i in 1 2 3; do for j in a b c; do echo $i$j; break 2; done; echo inner; done; echo outer
