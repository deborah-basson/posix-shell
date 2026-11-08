#!/bin/bash
# Test: while command substitution
i=0; while [ $(echo $i) -lt 3 ]; do echo $i; i=$((i+1)); done
