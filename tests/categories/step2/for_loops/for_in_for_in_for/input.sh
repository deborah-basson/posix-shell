#!/bin/bash
# Test: for in for in for
for a in 1; do for b in 2; do for c in 3; do echo $a$b$c; done; done; done
