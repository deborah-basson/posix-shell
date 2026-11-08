#!/bin/bash
# Test: function in condition
s() { false; }; if s; then echo yes; else echo no; fi
