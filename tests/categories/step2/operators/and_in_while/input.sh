#!/bin/bash
# Test: and in while
i=0; while [ $i -lt 2 ] && true; do echo $i; i=$((i+1)); done
