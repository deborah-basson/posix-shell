#!/bin/bash
# Test: while with command
i=0; while echo $i && [ $i -lt 3 ]; do i=$((i+1)); done
