#!/bin/bash
# Test: continue 999 one loop
i=0; while [ $i -lt 1 ]; do i=1; continue 999; echo no; done; echo yes
