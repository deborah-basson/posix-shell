#!/bin/bash
# Test: until status preserved
i=0; until [ $i -eq 1 ]; do i=1; false; done; echo $?
