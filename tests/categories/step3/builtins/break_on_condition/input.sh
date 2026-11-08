#!/bin/bash
# Test: break on condition
for i in {1..100}; do if [ $i -eq 42 ]; then break; fi; done; echo $i
