#!/bin/bash
# Test: redirect long content
echo aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa >long.txt; cat long.txt; rm long.txt
