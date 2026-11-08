#!/bin/bash
# Test: function return in loop
f() { for i in 1 2 3; do if [ $i -eq 2 ]; then return 20; fi; done; }; f; echo $?
