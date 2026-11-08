#!/bin/bash
# Test: negate for
for i in 1; do ! true; echo $?; done
