#!/bin/bash
# Test: subshell nested isolation
a=1; (a=2; (a=3; echo $a); echo $a); echo $a
