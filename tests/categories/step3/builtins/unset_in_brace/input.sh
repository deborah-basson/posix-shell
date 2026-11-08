#!/bin/bash
# Test: unset in brace
VAR=test; { unset VAR; }; echo $VAR
