#!/bin/bash
for i in 1 2; do
  for j in a b c; do
    [ $j = b ] && continue
    echo $i$j
  done
done
