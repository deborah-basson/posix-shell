#!/bin/bash
# Test: multiple redirects
echo test >f1.txt >f2.txt; cat f2.txt; rm f1.txt f2.txt
