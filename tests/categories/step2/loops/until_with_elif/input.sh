#!/bin/bash
# Test: until with elif
i=0; until [ $i -eq 5 ]; do if [ $i -eq 1 ]; then echo one; elif [ $i -eq 3 ]; then echo three; fi; i=$((i+1)); done
