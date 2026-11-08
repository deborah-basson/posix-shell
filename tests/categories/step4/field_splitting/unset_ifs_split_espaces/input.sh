#!/bin/bash
# Test: unset IFS split espaces
unset IFS; var="a  b  c"; echo $var
