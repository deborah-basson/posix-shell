#!/bin/bash
# Test: function in if condition
check() { true; }; if check; then echo ok; fi
