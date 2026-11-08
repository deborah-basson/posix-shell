#!/bin/bash
# Test: for with all operators
for i in 1 2 3; do true && echo $i || echo never; done
