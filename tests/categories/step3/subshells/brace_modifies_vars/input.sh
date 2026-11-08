#!/bin/bash
# Test: brace modifies vars
a=outer; { a=inner; echo $a; }; echo $a
