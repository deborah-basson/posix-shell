#!/bin/bash
# Test: function shift args
f() { echo $1; shift; echo $1; }; f a b c
