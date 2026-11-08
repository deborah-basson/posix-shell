#!/bin/bash
# Test: unset in loop
for i in 1 2 3; do V=$i; unset V; done; echo $V
