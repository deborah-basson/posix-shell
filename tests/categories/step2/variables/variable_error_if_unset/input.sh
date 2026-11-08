#!/bin/bash
# Test: variable error if unset
echo ${UNDEFINED:?error message} 2>&1 | grep -q error
