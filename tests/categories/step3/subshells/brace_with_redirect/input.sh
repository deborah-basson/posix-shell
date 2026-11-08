#!/bin/bash
# Test: brace with redirect
{ echo test; } >brace.txt; cat brace.txt; rm brace.txt
