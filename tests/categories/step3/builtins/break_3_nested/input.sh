#!/bin/bash
# Test: break 3 nested
i=0; while [ $i -lt 2 ]; do i=$((i+1)); j=0; while [ $j -lt 2 ]; do j=$((j+1)); k=0; while [ $k -lt 2 ]; do k=$((k+1)); echo $i$j$k; break 3; done; echo level2; done; echo level1; done; echo done
