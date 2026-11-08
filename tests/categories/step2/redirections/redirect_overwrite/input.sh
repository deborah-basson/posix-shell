#!/bin/bash
# Test: redirect overwrite
echo first >f.txt; echo second >f.txt; cat f.txt; rm f.txt
