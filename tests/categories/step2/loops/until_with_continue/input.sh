#!/bin/bash
# Test: until with continue
i=0; until [ $i -eq 5 ]; do i=$((i+1)); if [ $i -eq 3 ]; then continue; fi; echo $i; done
