#!/bin/bash
# Test: variable in all structures
x=test; if true; then while false; do echo never; done; for i in $x; do echo $i; done; fi
