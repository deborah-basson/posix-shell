#!/bin/bash
# Test: cd to ..
old=$(pwd); cd ..; new=$(pwd); cd $old; echo ok
