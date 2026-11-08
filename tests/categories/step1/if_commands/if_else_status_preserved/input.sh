#!/bin/bash
# Test: if else status preserved
if false; then true; else false; fi; echo $?
