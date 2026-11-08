#!/bin/bash
# Test: unset all vars loop
for i in {1..10}; do eval V$i=$i; done; for i in {1..10}; do unset V$i; done; echo $V1$V5$V10
