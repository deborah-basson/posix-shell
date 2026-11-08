#!/bin/bash
# Test: until immediate break
until false; do break; done; echo ok
