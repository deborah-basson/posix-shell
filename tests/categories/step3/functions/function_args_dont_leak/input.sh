#!/bin/bash
# Test: function args dont leak
f() { echo $1; }; f test; echo $1
