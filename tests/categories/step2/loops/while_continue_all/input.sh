#!/bin/bash
# Test: while continue all
i=0; while [ $i -lt 3 ]; do i=$((i+1)); continue; echo never; done; echo done
