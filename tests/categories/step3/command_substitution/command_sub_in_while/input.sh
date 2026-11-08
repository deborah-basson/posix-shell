#!/bin/bash
# Test: command sub in while
i=0; while [ $i -lt $(echo 3) ]; do echo $i; i=$((i+1)); done
