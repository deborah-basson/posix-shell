#!/bin/bash
# Test: keyword not aliased
alias if='echo wrong'
if true; then echo ok; fi
