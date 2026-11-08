#!/bin/bash
# Test: if elif last true
if false; then echo a; elif false; then echo b; elif false; then echo c; elif true; then echo d; fi
