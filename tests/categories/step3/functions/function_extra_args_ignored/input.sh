#!/bin/bash
# Test: function extra args ignored
two() { echo $1 $2; }; two a b c d
