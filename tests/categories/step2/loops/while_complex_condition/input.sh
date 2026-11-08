#!/bin/bash
# Test: while complex condition
i=0; while [ $i -lt 3 ] && [ $i -ge 0 ]; do echo $i; i=$((i+1)); done
