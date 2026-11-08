#!/bin/bash
# Test: status complex
true && false || true; echo $?
