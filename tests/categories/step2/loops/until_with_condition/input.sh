#!/bin/bash
# Test: until with && condition
i=0; until [ $i -eq 3 ] && true; do echo $i; i=$((i+1)); done
