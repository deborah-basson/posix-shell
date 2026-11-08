#!/bin/bash
# Test: unalias -a
alias x='echo x'
alias y='echo y'
unalias -a
echo ok
