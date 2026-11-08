#!/bin/bash
# Test: IFS= puis IFS=default
IFS=; var="a b"; x=$var; IFS=" "; echo $x
