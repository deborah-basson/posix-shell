#!/bin/bash
# Test: brace multiple vars
a=1; b=2; { a=10; b=20; echo $a $b; }; echo $a $b
