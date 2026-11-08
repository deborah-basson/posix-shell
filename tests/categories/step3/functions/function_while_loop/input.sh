#!/bin/bash
# Test: function while loop
w() { i=0; while [ $i -lt 2 ]; do echo $i; i=$((i+1)); done; }; w
