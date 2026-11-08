#!/bin/bash
# Test: IFS vide préserve tout
IFS=; var="a b c"; echo $var
