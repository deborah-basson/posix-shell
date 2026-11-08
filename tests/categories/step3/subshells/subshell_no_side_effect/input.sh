#!/bin/bash
# Test: subshell no side effect
x=original; (x=changed); echo $x
