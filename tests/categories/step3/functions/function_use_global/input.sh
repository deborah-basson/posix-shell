#!/bin/bash
# Test: function use global
x=global; f() { echo $x; }; f
