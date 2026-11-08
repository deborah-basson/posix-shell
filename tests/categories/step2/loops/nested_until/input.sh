#!/bin/bash
# Test: nested until
i=0; until [ $i -eq 2 ]; do j=0; until [ $j -eq 2 ]; do echo $i$j; j=$((j+1)); done; i=$((i+1)); done
