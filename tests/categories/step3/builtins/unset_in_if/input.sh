#!/bin/bash
# Test: unset in if
if true; then VAR=test; unset VAR; fi; echo $VAR
