#!/bin/bash
# Test: variable in while
x=0; while [ $x -lt 3 ]; do echo $x; x=$((x+1)); done
