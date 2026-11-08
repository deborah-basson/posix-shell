#!/bin/bash
# Test: function redirect out
r() { echo file; }; r > /tmp/func.txt; cat /tmp/func.txt; rm /tmp/func.txt
