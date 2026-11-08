#!/bin/bash
# Test: redirect elif branch
if false; then echo a >f.txt; elif true; then echo b >f.txt; fi; cat f.txt; rm f.txt
