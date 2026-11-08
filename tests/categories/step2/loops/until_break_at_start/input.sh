#!/bin/bash
# Test: until break at start
i=0; until [ $i -eq 10 ]; do break; i=$((i+1)); done; echo $i
