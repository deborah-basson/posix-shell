#!/bin/bash
# Test: pipe complex chain
echo test | cat | grep test | cat | cat
