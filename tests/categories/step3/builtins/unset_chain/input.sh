#!/bin/bash
# Test: unset chain
A=1; B=2; C=3; unset A; unset B; unset C; echo $A$B$C
