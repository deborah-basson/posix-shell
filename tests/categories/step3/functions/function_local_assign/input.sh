#!/bin/bash
# Test: function local assign
f() { x=local; echo $x; }; x=global; f; echo $x
