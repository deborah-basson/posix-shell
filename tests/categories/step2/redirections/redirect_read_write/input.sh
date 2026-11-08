#!/bin/bash
# Test: redirect read write
echo test <>f.txt; cat f.txt; rm f.txt
