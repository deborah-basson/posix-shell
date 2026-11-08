#!/bin/bash
# Test: IFS vide préserve espaces multiples
IFS=; var="a  b    c"; echo $var
