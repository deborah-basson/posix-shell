#!/bin/bash
# Test: function redefine call both
g() { echo first; }; g; g() { echo second; }; g
