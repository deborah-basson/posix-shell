#!/bin/bash
# Test: while with || condition
i=0; while [ $i -lt 3 ] || false; do echo $i; i=$((i+1)); if [ $i -eq 3 ]; then break; fi; done
