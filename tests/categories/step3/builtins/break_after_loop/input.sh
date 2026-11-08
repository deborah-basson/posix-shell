#!/bin/bash
# Test: break after loop
while false; do echo hi; done; break 2>&1
