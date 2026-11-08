#!/bin/bash
# Test: until twenty iterations
i=0; until [ $i -eq 20 ]; do echo $i; i=$((i+1)); done
