#!/bin/bash
# Test: function mutual recursion
even() { if [ $1 -eq 0 ]; then return 0; fi; odd $(($1 - 1)); }; odd() { if [ $1 -eq 0 ]; then return 1; fi; even $(($1 - 1)); }; even 4; echo $?
