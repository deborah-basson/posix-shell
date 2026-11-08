#!/bin/bash
# Test: unset after assignment
VAR=test; unset VAR; VAR=new; echo $VAR
