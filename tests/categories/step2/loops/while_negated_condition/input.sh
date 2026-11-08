#!/bin/bash
# Test: while negated condition
i=0; while ! [ $i -eq 3 ]; do echo $i; i=$((i+1)); done
