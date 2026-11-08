#!/bin/bash
# Test: until negated condition
i=0; until ! [ $i -lt 3 ]; do echo $i; i=$((i+1)); done
