#!/bin/bash
# Test: break in subshell loop
(for i in 1 2 3; do echo $i; if [ $i -eq 2 ]; then break; fi; done)
