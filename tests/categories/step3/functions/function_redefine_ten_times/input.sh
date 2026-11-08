#!/bin/bash
# Test: function redefine ten times
f() { echo 0; }; f() { echo 1; }; f() { echo 2; }; f() { echo 3; }; f() { echo 4; }; f() { echo 5; }; f() { echo 6; }; f() { echo 7; }; f() { echo 8; }; f() { echo 9; }; f
