#!/bin/bash
# Test: while alternating break continue
i=0; while [ $i -lt 10 ]; do i=$((i+1)); if [ $((i % 2)) -eq 0 ]; then continue; fi; if [ $i -eq 7 ]; then break; fi; echo $i; done
