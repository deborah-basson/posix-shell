#!/bin/bash
# Test: variable hundred assign
for i in {1..100}; do eval v$i=$i; done; echo $v50 $v100
