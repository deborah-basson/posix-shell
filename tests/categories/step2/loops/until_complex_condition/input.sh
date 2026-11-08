#!/bin/bash
# Test: until complex condition
i=0; until [ $i -eq 3 ] || [ $i -gt 5 ]; do echo $i; i=$((i+1)); done
