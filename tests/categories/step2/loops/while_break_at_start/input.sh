#!/bin/bash
# Test: while break at start
i=0; while [ $i -lt 10 ]; do break; i=$((i+1)); done; echo $i
