#!/bin/bash
# Test: unset in while
i=0; while [ $i -lt 3 ]; do X=$i; unset X; i=$((i+1)); done; echo $X
