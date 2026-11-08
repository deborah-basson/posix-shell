#!/bin/bash
# Test: while exit status chain
while false; do echo never; done; while false; do echo never; done; echo ok
