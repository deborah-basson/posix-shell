#!/bin/bash
# Test: function if elif else
f() { if false; then echo a; elif true; then echo b; else echo c; fi; }; f
