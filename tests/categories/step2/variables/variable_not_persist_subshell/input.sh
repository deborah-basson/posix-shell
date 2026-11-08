#!/bin/bash
# Test: variable not persist subshell
x=test; (x=changed); echo $x
