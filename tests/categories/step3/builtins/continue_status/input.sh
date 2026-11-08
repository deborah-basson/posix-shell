#!/bin/bash
# Test: continue status
for i in 1 2 3; do if [ $i -eq 2 ]; then continue; fi; echo $i; done; echo $?
