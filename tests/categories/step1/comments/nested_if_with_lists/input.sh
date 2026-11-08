#!/bin/bash
# Test: nested if with lists
if true; then echo 1; if false; then echo 2; else echo 3; fi; echo 4; fi
