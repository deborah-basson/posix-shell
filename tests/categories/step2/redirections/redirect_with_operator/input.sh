#!/bin/bash
# Test: redirect with && operator
true && echo ok >and.txt; cat and.txt; rm and.txt
