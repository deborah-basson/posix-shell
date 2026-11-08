#!/bin/bash
# Test: function nested with return
a() { return 5; }; b() { a; return $?; }; b; echo $?
