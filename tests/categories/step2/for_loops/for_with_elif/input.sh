#!/bin/bash
# Test: for with elif
for i in 1 2 3; do if [ $i -eq 1 ]; then echo one; elif [ $i -eq 2 ]; then echo two; else echo other; fi; done
