#!/bin/bash
# Test: very nested mixed
a=0; (a=1; (a=2; { a=3; (a=4; echo $a); echo $a; }; echo $a); echo $a); echo $a
