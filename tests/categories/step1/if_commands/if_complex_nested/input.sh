#!/bin/bash
# Test: if complex nested
if true; then if false; then echo a; elif true; then echo b; else echo c; fi; echo d; fi
