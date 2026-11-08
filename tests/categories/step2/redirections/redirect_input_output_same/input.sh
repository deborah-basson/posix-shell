#!/bin/bash
# Test: redirect input output same
echo test >f.txt; cat <f.txt >f2.txt; cat f2.txt; rm f.txt f2.txt
