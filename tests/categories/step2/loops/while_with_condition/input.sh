#!/bin/bash
# Test: while with && condition
i=0; while [ $i -lt 3 ] && true; do echo $i; i=$((i+1)); done
