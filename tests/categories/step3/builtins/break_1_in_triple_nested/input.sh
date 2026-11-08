#!/bin/bash
# Test: break 1 in triple nested
for a in 1 2; do for b in 1 2; do for c in 1 2 3; do echo $a$b$c; break 1; done; echo after; done; done
