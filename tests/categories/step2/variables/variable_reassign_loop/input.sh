#!/bin/bash
# Test: variable reassign loop
x=0; for i in 1 2 3; do x=$i; done; echo $x
