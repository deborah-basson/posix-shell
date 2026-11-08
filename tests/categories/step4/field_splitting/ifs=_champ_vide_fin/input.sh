#!/bin/bash
# Test: IFS=: champ vide fin
IFS=:; var="a:b:"; echo $var
