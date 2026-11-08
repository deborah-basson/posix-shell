#!/bin/bash
# Test: if false true pattern
if false; then echo no; else if true; then echo yes; fi; fi
