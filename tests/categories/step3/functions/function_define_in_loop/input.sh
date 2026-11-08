#!/bin/bash
# Test: function define in loop
for i in 1; do bar() { echo $i; }; done; bar
