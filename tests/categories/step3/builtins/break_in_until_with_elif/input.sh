#!/bin/bash
# Test: break in until with elif
i=0; until [ $i -eq 10 ]; do i=$((i+1)); if [ $i -eq 2 ]; then echo two; elif [ $i -eq 4 ]; then break; else echo $i; fi; done
