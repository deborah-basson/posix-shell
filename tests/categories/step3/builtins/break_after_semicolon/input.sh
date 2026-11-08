#!/bin/bash
# Test: break after semicolon
while true; do echo a; echo b; break; echo c; done
