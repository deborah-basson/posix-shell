#!/bin/bash
# Test: until with break
i=0; until false; do echo $i; i=$((i+1)); if [ $i -eq 3 ]; then break; fi; done
