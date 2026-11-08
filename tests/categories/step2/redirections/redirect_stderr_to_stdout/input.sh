#!/bin/bash
# Test: redirect stderr to stdout
echo error >&2 2>&1 | cat
