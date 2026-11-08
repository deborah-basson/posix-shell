#!/bin/bash
# Test: for with if
for i in 1 2 3 4 5; do if [ $i -eq 3 ]; then echo three; fi; done
