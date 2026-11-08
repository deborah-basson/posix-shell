#!/bin/bash
# Test: variable in subshell
x=outer; (x=inner; echo $x); echo $x
