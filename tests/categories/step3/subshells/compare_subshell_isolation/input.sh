#!/bin/bash
# Test: compare subshell isolation
a=sh; (a=42; echo $a); echo $a
