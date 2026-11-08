#!/bin/bash
# Test: until with if
i=0; until [ $i -eq 5 ]; do if [ $i -eq 2 ]; then echo two; fi; i=$((i+1)); done
