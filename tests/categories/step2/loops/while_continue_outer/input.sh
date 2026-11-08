#!/bin/bash
# Test: while continue outer
i=0; while [ $i -lt 3 ]; do i=$((i+1)); j=0; while [ $j -lt 2 ]; do j=$((j+1)); if [ $i -eq 2 ]; then continue 2; fi; echo $i$j; done; echo after; done
