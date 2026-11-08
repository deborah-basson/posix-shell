#!/bin/bash
# Test: for immediate break
for i in 1 2 3 4 5; do break; echo never; done; echo ok
