#!/bin/bash
# Test: block in while
i=0; while [ $i -lt 2 ]; do { i=$((i+1)); echo $i; }; done
