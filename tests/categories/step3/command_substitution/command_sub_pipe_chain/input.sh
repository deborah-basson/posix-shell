#!/bin/bash
# Test: command sub pipe chain
echo $(echo test | cat | cat | cat)
