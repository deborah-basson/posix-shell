#!/bin/bash
# Test: nested loops if
for i in 1 2; do if [ $i -eq 1 ]; then echo one; else echo two; fi; done
