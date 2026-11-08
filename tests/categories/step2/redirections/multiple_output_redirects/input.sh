#!/bin/bash
# Test: multiple output redirects
echo test >f1.txt >f2.txt >f3.txt; cat f3.txt; rm f1.txt f2.txt f3.txt
