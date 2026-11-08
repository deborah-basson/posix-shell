#!/bin/bash
# Test: block redir append
echo a > /tmp/f.txt; { echo b; } >> /tmp/f.txt; cat /tmp/f.txt; rm /tmp/f.txt
