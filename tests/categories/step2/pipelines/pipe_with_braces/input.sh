#!/bin/bash
# Test: pipe with braces
echo test | { cat | cat; }
