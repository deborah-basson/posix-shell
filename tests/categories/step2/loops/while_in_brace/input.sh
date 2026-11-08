#!/bin/bash
# Test: while in brace
{ i=0; while [ $i -lt 3 ]; do echo $i; i=$((i+1)); done; }
