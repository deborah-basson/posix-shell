#!/bin/bash
# Test: command sub preserves status
x=$(false); echo $?
