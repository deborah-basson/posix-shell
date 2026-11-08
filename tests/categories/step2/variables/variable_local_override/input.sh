#!/bin/bash
# Test: variable local override
x=global; f() { x=local; echo $x; }; f; echo $x
