#!/bin/bash
# Test: if true false pattern
if true; then if false; then echo no; else echo yes; fi; fi
