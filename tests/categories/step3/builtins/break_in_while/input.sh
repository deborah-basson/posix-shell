#!/bin/bash
# Test: break in while
i=0; while true; do i=$((i+1)); echo $i; if [ $i -eq 3 ]; then break; fi; done; echo done
