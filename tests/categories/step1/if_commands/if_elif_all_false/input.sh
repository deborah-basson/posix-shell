#!/bin/bash
# Test: if elif all false
if false; then echo a; elif false; then echo b; elif false; then echo c; elif false; then echo d; fi
