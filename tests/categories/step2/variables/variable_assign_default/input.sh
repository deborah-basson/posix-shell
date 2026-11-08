#!/bin/bash
# Test: variable assign default
echo ${UNDEFINED:=value}; echo $UNDEFINED
