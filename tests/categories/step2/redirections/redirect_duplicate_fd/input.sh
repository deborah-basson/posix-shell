#!/bin/bash
# Test: redirect duplicate fd
echo test 1>&2 2>&1 | cat
