#!/bin/bash
# Test: continue skips rest
for i in 1 2 3; do if [ $i -eq 2 ]; then continue; fi; echo $i; done
