#!/bin/bash
# Test: redefine alias
alias x='echo first'
alias x='echo second'
x
