#!/bin/bash
# Test: while with elif
i=0; while [ $i -lt 5 ]; do if [ $i -eq 1 ]; then echo one; elif [ $i -eq 3 ]; then echo three; fi; i=$((i+1)); done
