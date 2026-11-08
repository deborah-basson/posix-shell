#!/bin/bash
# Test: redirect with variables
x=test; echo $x >var.txt; cat var.txt; rm var.txt
