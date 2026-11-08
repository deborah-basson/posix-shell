#!/bin/bash
# Test: pipe echo to awk
echo 'a b c' | awk '{print $2}'
