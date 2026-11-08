#!/bin/bash
# Test: mixed break continue complex
for i in 1 2 3 4 5 6 7 8 9 10; do if [ $i -lt 3 ]; then continue; fi; if [ $i -gt 7 ]; then break; fi; echo $i; done
