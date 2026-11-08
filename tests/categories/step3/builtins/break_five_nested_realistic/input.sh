#!/bin/bash
# Test: break five nested realistic
for a in 1 2; do for b in 1 2; do for c in 1 2; do for d in 1 2; do for e in 1 2; do if [ $a$b$c$d$e = 11111 ]; then echo found; break 5; fi; done; done; done; done; done; echo done
