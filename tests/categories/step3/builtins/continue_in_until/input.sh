#!/bin/bash
# Test: continue in until
i=0; until [ $i -eq 4 ]; do i=$((i+1)); if [ $i -eq 2 ]; then continue; fi; echo $i; done
