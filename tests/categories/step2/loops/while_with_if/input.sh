#!/bin/bash
# Test: while with if
i=0; while [ $i -lt 5 ]; do if [ $i -eq 2 ]; then echo two; fi; i=$((i+1)); done
