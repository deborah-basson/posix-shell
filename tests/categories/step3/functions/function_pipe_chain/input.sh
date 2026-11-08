#!/bin/bash
# Test: function pipe chain
a() { echo x; }; b() { cat; }; a | b | cat
