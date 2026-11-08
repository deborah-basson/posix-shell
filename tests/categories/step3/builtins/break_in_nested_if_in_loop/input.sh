#!/bin/bash
# Test: break in nested if in loop
for i in 1 2 3; do if true; then if [ $i -eq 2 ]; then break; fi; fi; echo $i; done
