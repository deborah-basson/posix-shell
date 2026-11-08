#!/bin/bash
# Test: compare brace modification
a=sh; { a=42; echo $a; }; echo $a
