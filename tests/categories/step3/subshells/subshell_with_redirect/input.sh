#!/bin/bash
# Test: subshell with redirect
(echo test) >sub.txt; cat sub.txt; rm sub.txt
