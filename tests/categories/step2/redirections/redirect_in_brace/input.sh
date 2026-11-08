#!/bin/bash
# Test: redirect in brace
{ echo test >brace.txt; }; cat brace.txt; rm brace.txt
