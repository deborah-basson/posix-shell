#!/bin/bash
# Test: break ten loops
for a in 1; do for b in 1; do for c in 1; do for d in 1; do for e in 1; do for f in 1; do for g in 1; do for h in 1; do for i in 1; do for j in 1; do echo deep; break 10; done; done; done; done; done; done; done; done; done; done; echo ok
