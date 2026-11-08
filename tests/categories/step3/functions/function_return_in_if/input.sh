#!/bin/bash
# Test: function return in if
f() { if true; then return 10; fi; }; f; echo $?
