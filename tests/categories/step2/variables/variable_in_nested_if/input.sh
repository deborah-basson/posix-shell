#!/bin/bash
# Test: variable in nested if
x=test; if true; then if [ $x = test ]; then echo ok; fi; fi
