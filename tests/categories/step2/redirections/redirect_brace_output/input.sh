#!/bin/bash
# Test: redirect brace output
{ echo brace; } >brace.txt; cat brace.txt; rm brace.txt
