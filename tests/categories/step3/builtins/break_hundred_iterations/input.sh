#!/bin/bash
# Test: break hundred iterations
i=0; while [ $i -lt 100 ]; do i=$((i+1)); if [ $i -eq 50 ]; then break; fi; done; echo $i
