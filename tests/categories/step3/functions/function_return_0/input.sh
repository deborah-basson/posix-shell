#!/bin/bash
# Test: function return 0
f() { return 0; }; f; echo $?
