#!/bin/bash
# Test: while status preserved
i=0; while [ $i -lt 1 ]; do i=1; false; done; echo $?
