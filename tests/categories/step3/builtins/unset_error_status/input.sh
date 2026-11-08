#!/bin/bash
# Test: unset error status
unset -x VAR 2>&1; echo $?
