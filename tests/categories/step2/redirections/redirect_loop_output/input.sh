#!/bin/bash
# Test: redirect loop output
for i in 1 2 3 4 5; do echo $i; done >loop.txt; cat loop.txt; rm loop.txt
