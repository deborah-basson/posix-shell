#!/bin/bash
# Test: redirect chain
echo 1 >f.txt; cat f.txt >f2.txt; cat f2.txt; rm f.txt f2.txt
