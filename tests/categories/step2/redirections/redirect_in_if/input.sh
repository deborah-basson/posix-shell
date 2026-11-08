#!/bin/bash
# Test: redirect in if
if true; then echo test >if.txt; fi; cat if.txt; rm if.txt
