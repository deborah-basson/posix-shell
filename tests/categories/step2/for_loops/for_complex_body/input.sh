#!/bin/bash
# Test: for complex body
for i in 1 2 3; do if [ $i -eq 1 ]; then echo first; elif [ $i -eq 2 ]; then for j in a b; do echo $j; done; else echo last; fi; done
