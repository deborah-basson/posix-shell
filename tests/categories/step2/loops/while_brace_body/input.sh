#!/bin/bash
# Test: while brace body
i=0; while [ $i -lt 3 ]; do { echo $i; i=$((i+1)); }; done
