#!/bin/bash
# Test: redirect both &>
echo both &>both.txt; cat both.txt; rm both.txt
