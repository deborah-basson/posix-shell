#!/bin/bash
# Test: while true status
i=0; while [ $i -lt 1 ]; do i=1; true; done; echo $?
