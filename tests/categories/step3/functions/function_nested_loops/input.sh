#!/bin/bash
# Test: function nested loops
nl() { for i in 1 2; do for j in a b; do echo $i$j; done; done; }; nl
