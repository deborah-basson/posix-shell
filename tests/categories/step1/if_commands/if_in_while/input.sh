#!/bin/bash
# Test: if in while
i=0; while [ $i -lt 3 ]; do if [ $i -eq 1 ]; then echo one; fi; i=$((i+1)); done
