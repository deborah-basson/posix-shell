#!/bin/bash
# Test: redirect multiple fd
echo test 1>out.txt 2>err.txt; cat out.txt; rm out.txt err.txt
