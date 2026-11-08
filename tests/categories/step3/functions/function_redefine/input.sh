#!/bin/bash
# Test: function redefine
f() { echo one; }; f() { echo two; }; f
