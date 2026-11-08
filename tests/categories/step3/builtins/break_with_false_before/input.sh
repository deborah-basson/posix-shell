#!/bin/bash
# Test: break with false before
for i in 1 2 3; do false; if [ $i -eq 2 ]; then break; fi; done; echo $?
