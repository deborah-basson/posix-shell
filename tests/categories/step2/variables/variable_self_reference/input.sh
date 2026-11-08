#!/bin/bash
# Test: variable self reference
x=old; x=$x-new; echo $x
