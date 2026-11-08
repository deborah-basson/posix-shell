#!/bin/bash
# Test: continue 2 levels
i=0; while [ $i -lt 2 ]; do i=$((i+1)); echo outer$i; j=0; while [ $j -lt 2 ]; do j=$((j+1)); echo inner$j; continue 2; done; echo after; done; echo done
