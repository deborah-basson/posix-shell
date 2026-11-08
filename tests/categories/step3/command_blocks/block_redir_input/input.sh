#!/bin/bash
# Test: block redir input
echo content > /tmp/in.txt; { cat /tmp/in.txt; }; rm /tmp/in.txt
