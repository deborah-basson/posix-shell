#!/bin/bash
# Test: until continue all
i=0; until [ $i -eq 3 ]; do i=$((i+1)); continue; echo never; done; echo done
