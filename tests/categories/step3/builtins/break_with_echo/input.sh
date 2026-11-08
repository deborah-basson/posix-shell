#!/bin/bash
# Test: break with echo
for i in 1 2 3; do echo $i; break; echo never; done
