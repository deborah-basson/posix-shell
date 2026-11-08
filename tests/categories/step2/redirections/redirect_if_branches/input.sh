#!/bin/bash
# Test: redirect if branches
if true; then echo yes >f.txt; else echo no >f.txt; fi; cat f.txt; rm f.txt
