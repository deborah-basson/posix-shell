#!/bin/bash
# Test: block in until
i=0; until [ $i -eq 2 ]; do { echo $i; i=$((i+1)); }; done
