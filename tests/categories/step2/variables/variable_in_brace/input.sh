#!/bin/bash
# Test: variable in brace
x=outer; { x=inner; echo $x; }; echo $x
