#!/bin/bash
# Test: mixed structures
if true; then true; fi; echo a; if false; then echo b; else echo c; fi
