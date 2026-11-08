#!/bin/bash
# Test: command sub in redirect
echo test > $(echo file.txt); cat file.txt; rm file.txt
