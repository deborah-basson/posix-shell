#!/bin/bash
# Test: until command substitution
i=0; until [ $(echo $i) -eq 3 ]; do echo $i; i=$((i+1)); done
