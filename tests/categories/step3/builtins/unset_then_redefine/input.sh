#!/bin/bash
# Test: unset then redefine
VAR=a; unset VAR; VAR=b; echo $VAR
