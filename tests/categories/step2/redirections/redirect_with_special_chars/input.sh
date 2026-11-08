#!/bin/bash
# Test: redirect with special chars
echo '@#$%' >spec.txt; cat spec.txt; rm spec.txt
