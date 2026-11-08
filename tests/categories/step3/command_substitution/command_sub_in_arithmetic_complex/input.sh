#!/bin/bash
# Test: command sub in arithmetic complex
echo $(( $(echo 10) * $(echo 5) + $(echo 3) ))
