#!/bin/bash
# Test: break in until
i=0; until [ $i -eq 5 ]; do i=$((i+1)); echo $i; if [ $i -eq 2 ]; then break; fi; done; echo done
