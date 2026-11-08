#!/bin/bash
# Test: unset with export
export VAR=test; unset VAR; echo $VAR
