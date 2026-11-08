#!/bin/bash
# Test: same line no expansion
alias foo=echo; foo test 2>&1 || echo failed
