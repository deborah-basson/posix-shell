#!/bin/bash
# Test: function in loop body
f() { echo x; }; for i in 1 2; do f; done
