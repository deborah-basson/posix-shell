#!/bin/bash
# Test: redirect in subshell
(echo test >sub.txt); cat sub.txt; rm sub.txt
