#!/bin/bash
# Test: until multiline
i=0 until [ $i -eq 3 ] do echo $i i=$((i+1)) done
