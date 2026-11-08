#!/bin/bash
# Test: cd - swaps dirs
cd /tmp; cd /var; cd - 2>&1 | head -1
