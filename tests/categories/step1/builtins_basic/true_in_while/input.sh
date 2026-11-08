#!/bin/bash
# Test: true in while
i=0; while true; do i=$((i+1)); if [ $i -eq 2 ]; then break; fi; done; echo $i
