#!/bin/bash
# Test: until in subshell
(i=0; until [ $i -eq 3 ]; do echo $i; i=$((i+1)); done)
