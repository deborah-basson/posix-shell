#!/bin/bash
# Test: for immediate continue
for i in 1 2 3; do continue; echo never; done; echo ok
