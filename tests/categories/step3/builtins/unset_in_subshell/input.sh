#!/bin/bash
# Test: unset in subshell
VAR=outer; (VAR=inner; unset VAR; echo $VAR); echo $VAR
