#!/bin/bash
# Test: for five nested
for a in 1; do for b in 2; do for c in 3; do for d in 4; do for e in 5; do echo $a$b$c$d$e; done; done; done; done; done
