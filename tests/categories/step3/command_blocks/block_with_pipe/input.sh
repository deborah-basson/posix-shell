#!/bin/bash
# Test: block with pipe
{ echo a; echo b; } | grep a
