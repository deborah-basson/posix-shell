#!/bin/bash
# Test: complex nesting mixed
a=1; { (a=2; echo $a); echo $a; }; echo $a
