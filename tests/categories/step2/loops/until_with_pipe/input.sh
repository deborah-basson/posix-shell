#!/bin/bash
# Test: until with pipe
i=0; until [ $i -eq 3 ]; do echo $i | cat; i=$((i+1)); done
