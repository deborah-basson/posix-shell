#!/bin/bash
# Test: redirect both streams separate
echo out >out.txt; echo err >&2 2>err.txt; cat out.txt; cat err.txt; rm out.txt err.txt
