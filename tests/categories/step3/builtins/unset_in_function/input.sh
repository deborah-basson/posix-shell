#!/bin/bash
# Test: unset in function
f() { local X=test; unset X; echo $X; }; f
