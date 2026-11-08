#!/bin/bash
# Test: triple nested if with elif
if true; then if false; then echo a; elif true; then if true; then echo b; fi; fi; fi
