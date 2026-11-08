#!/bin/bash
# Test: continue after semicolon
i=0; while [ $i -lt 2 ]; do i=$((i+1)); echo a; continue; echo b; done
