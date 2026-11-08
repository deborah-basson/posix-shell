#!/bin/bash
# Test: if command not found
if nonexistentcmd123; then echo never; else echo ok; fi
