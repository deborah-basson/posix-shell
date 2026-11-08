#!/bin/bash
# Test: while nested three levels
i=0; while [ $i -lt 2 ]; do j=0; while [ $j -lt 2 ]; do k=0; while [ $k -lt 2 ]; do echo $i$j$k; k=$((k+1)); done; j=$((j+1)); done; i=$((i+1)); done
