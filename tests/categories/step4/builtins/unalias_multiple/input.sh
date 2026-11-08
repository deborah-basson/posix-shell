#!/bin/bash
# Test: unalias multiple
alias a='echo a'
alias b='echo b'
unalias a b
echo done
