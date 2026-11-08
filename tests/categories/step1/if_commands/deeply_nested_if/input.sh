#!/bin/bash
# Test: deeply nested if
if true; then if true; then if true; then if true; then echo deep; fi; fi; fi; fi
