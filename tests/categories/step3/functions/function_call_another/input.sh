#!/bin/bash
# Test: function call another
a() { echo A; }; b() { a; }; b
