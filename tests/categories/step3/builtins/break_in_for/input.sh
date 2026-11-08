#!/bin/bash
# Test: break in for
for i in 1 2 3 4 5; do echo $i; if [ $i -eq 3 ]; then break; fi; done; echo done
