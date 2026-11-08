#!/bin/bash
# Test: redirect input nonexistent
cat <nonexistent.txt 2>&1 | grep -q 'No such file'
