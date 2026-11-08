#!/bin/bash
# Test: function redirect in func
r() { echo inner > /tmp/func2.txt; }; r; cat /tmp/func2.txt; rm /tmp/func2.txt
