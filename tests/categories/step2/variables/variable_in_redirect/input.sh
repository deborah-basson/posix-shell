#!/bin/bash
# Test: variable in redirect
x=test; echo $x >var.txt; cat var.txt; rm var.txt
