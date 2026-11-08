#!/bin/bash
# Test: nested while
i=0; while [ $i -lt 2 ]; do j=0; while [ $j -lt 2 ]; do echo $i$j; j=$((j+1)); done; i=$((i+1)); done
