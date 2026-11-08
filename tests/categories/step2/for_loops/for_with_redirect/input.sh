#!/bin/bash
# Test: for with redirect
for i in 1 2 3; do echo $i; done >for.txt; cat for.txt; rm for.txt
