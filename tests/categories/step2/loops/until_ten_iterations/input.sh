#!/bin/bash
# Test: until ten iterations
i=0; until [ $i -eq 10 ]; do echo $i; i=$((i+1)); done
