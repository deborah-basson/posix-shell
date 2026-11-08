#!/bin/bash
# Test: redirect stderr 2>
echo error >&2 2>err.txt; cat err.txt; rm err.txt
