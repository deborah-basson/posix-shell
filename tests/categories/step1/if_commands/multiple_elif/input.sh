#!/bin/bash
# Test: multiple elif
if false; then echo a; elif false; then echo b; elif true; then echo c; fi
