#!/bin/bash
# Test: function with brace
f() { { echo brace; }; echo main; }; f
