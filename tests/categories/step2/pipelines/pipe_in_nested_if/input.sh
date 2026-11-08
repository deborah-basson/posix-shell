#!/bin/bash
# Test: pipe in nested if
if true; then if echo test | grep test; then echo ok; fi; fi
