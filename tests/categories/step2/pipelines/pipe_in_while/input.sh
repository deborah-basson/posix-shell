#!/bin/bash
# Test: pipe in while
i=0; while [ $i -lt 2 ]; do echo $i | cat; i=$((i+1)); done
