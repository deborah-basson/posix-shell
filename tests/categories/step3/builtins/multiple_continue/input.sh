#!/bin/bash
# Test: multiple continue
for i in 1 2 3 4 5; do if [ $i -eq 2 ]; then continue; fi; if [ $i -eq 4 ]; then continue; fi; echo $i; done
