#!/bin/bash
# Test: while with pipe
i=0; while [ $i -lt 3 ]; do echo $i | cat; i=$((i+1)); done
