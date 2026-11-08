#!/bin/bash
# Test: redirect fd 2 only
echo test 2>err.txt; cat err.txt 2>&1 || true; rm err.txt
