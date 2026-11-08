#!/bin/bash
# Test: break in block
while true; do { echo hi; break; }; echo no; done
