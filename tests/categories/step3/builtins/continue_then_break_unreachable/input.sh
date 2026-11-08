#!/bin/bash
# Test: continue then break unreachable
for i in 1 2 3; do continue; break; echo $i; done
