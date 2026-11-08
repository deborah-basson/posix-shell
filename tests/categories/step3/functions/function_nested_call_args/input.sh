#!/bin/bash
# Test: function nested call args
a() { echo A:$1; b x; echo A:$1; }; b() { echo B:$1; }; a y
