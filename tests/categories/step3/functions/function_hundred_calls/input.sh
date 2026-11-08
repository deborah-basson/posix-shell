#!/bin/bash
# Test: function hundred calls
f() { echo x; }; i=0; while [ $i -lt 100 ]; do f > /dev/null; i=$((i+1)); done; echo $i
