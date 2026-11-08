#!/bin/bash
# Test: if pipe in condition
if echo test | grep test; then echo ok; fi
