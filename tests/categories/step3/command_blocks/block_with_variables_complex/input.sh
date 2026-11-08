#!/bin/bash
# Test: block with variables complex
A=1; { B=2; { C=3; }; }; echo $A$B$C
