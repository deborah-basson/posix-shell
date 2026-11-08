#!/bin/bash
# Test: unalias existing
alias foo='echo bar'
unalias foo
echo ok
