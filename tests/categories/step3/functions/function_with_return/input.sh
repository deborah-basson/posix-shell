#!/bin/bash
# Test: function with return
f() { return 42; }; f; echo $?
