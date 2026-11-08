#!/bin/bash
# Test: export in loop
for i in 1; do export Y=$i; done; echo $Y
