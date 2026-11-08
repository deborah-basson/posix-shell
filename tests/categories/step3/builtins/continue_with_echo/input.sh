#!/bin/bash
# Test: continue with echo
for i in 1 2 3; do echo before; continue; echo after; done
