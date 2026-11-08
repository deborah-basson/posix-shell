#!/bin/bash
# Test: command sub with function
f() { echo result; }; echo $(f)
