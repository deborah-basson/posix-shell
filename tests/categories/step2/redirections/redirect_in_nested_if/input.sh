#!/bin/bash
# Test: redirect in nested if
if true; then if true; then echo nested >n.txt; fi; fi; cat n.txt; rm n.txt
