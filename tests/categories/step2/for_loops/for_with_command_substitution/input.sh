#!/bin/bash
# Test: for with command substitution
for i in $(echo a b c); do echo $i; done
