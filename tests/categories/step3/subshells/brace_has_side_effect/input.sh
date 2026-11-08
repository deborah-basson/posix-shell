#!/bin/bash
# Test: brace has side effect
x=original; { x=changed; }; echo $x
