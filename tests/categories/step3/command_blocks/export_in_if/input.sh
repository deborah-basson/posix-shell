#!/bin/bash
# Test: export in if
if true; then export X=test; fi; echo $X
