#!/bin/bash
# Test: function definition with alias
alias funcdec='foo('
funcdec) { echo ok; }
foo
