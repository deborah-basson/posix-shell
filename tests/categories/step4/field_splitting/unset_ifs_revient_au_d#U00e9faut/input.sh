#!/bin/bash
# Test: unset IFS revient au défaut
IFS=:; unset IFS; var="a b c"; echo $var
