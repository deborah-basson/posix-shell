#!/bin/bash
# Test: until three nested
i=0; until [ $i -eq 2 ]; do j=0; until [ $j -eq 2 ]; do k=0; until [ $k -eq 2 ]; do echo $i$j$k; k=$((k+1)); done; j=$((j+1)); done; i=$((i+1)); done
