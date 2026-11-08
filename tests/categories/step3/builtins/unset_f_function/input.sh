#!/bin/bash
# Test: unset -f function
f() { echo test; }; unset -f f; f 2>&1 | grep -q command
