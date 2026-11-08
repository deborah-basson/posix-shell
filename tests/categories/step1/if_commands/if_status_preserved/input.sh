#!/bin/bash
# Test: if status preserved
if true; then false; fi; echo $?
