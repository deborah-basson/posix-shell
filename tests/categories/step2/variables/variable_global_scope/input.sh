#!/bin/bash
# Test: variable global scope
x=global; f() { echo $x; }; f
