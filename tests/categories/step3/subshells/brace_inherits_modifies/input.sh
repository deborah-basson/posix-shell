#!/bin/bash
# Test: brace inherits modifies
x=hello; { x=world; echo $x; }; echo $x
