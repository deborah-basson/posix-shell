#!/bin/bash
# Test: block modifies var
VAR=before; { VAR=after; }; echo $VAR
