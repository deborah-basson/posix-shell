#!/bin/bash
# Test: continue in block
for i in 1 2 3; do { if [ $i -eq 2 ]; then continue; fi; echo $i; }; done
