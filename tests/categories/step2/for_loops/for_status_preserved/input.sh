#!/bin/bash
# Test: for status preserved
for i in 1; do false; done; echo $?
