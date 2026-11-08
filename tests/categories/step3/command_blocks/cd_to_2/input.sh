#!/bin/bash
# Test: cd to .
old=$(pwd); cd .; new=$(pwd); [ $old = $new ] && echo ok
