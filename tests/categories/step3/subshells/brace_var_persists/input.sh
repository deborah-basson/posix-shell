#!/bin/bash
# Test: brace var persists
a=1; { a=2; }; echo $a
