#!/bin/bash
# Test: function with subshell
f() { (echo sub); echo main; }; f
