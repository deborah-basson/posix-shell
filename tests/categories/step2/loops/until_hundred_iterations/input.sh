#!/bin/bash
# Test: until hundred iterations
i=0; until [ $i -eq 100 ]; do i=$((i+1)); done; echo $i
