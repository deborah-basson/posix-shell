#!/bin/bash
# Test: elif with multiple commands
if false; then echo a; elif true; then echo b; echo c; fi
