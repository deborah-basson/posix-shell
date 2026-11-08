#!/bin/bash
# Test: if in until
i=0; until [ $i -eq 3 ]; do if [ $i -eq 1 ]; then echo one; fi; i=$((i+1)); done
