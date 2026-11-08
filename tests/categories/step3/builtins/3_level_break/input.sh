#!/bin/bash
# Test: 3 level break
for a in 1 2; do for b in 1 2; do for c in 1 2; do echo $a$b$c; if [ $a$b$c = 111 ]; then break 3; fi; done; done; done; echo done
