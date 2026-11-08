#!/bin/bash
# Test: redirect in for
for i in a b c; do echo $i; done >for.txt; cat for.txt; rm for.txt
