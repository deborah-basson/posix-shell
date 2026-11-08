#!/bin/bash
# Test: IFS=: avec quotes pas de split
IFS=:; var="a:b:c"; echo "$var"
