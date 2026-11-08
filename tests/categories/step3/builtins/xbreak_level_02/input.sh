#!/bin/bash
for i in 1 2; do
  for j in a b; do
    echo $i$j
    break
  done
done
