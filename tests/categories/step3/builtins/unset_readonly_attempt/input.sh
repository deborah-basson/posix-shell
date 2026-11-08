#!/bin/bash
# Test: unset readonly attempt
readonly VAR=test; unset VAR 2>&1; echo $VAR
