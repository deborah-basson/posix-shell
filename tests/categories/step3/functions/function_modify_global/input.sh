#!/bin/bash
# Test: function modify global
x=before; f() { x=after; }; f; echo $x
