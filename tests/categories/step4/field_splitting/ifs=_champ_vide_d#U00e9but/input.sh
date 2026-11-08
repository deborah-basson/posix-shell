#!/bin/bash
# Test: IFS=: champ vide début
IFS=:; var=":a:b"; echo $var
