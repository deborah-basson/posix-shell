#!/bin/bash
# Test: redirect subshell output
(echo sub) >sub.txt; cat sub.txt; rm sub.txt
