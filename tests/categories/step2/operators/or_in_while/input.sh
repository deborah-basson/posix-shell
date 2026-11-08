#!/bin/bash
# Test: or in while
i=0; while [ $i -lt 2 ] || false; do echo $i; i=$((i+1)); if [ $i -eq 2 ]; then break; fi; done
