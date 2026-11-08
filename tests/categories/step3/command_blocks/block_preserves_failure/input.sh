#!/bin/bash
# Test: block preserves failure
{ false; }; echo $?
