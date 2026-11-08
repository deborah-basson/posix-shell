#!/bin/bash
# Test: break then continue unreachable
for i in 1 2 3; do if [ $i -eq 2 ]; then break; fi; continue; echo $i; done
