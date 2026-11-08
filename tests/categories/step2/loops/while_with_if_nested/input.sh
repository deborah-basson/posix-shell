#!/bin/bash
# Test: while with if nested
i=0; while [ $i -lt 3 ]; do if [ $i -eq 0 ]; then echo zero; elif [ $i -eq 1 ]; then echo one; else echo other; fi; i=$((i+1)); done
