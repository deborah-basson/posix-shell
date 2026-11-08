#!/bin/bash
# Test: variable in function
f() { echo $1; }; x=arg; f $x
