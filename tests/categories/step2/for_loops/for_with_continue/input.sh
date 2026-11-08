#!/bin/bash
# Test: for with continue
for i in 1 2 3 4 5; do if [ $i -eq 3 ]; then continue; fi; echo $i; done
