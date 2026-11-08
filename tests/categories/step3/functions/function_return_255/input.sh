#!/bin/bash
# Test: function return 255
f() { return 255; }; f; echo $?
