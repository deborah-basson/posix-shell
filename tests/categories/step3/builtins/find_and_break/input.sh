#!/bin/bash
# Test: find and break
for file in /etc/passwd /etc/hosts /etc/group; do if [ -f $file ]; then echo found $file; break; fi; done
