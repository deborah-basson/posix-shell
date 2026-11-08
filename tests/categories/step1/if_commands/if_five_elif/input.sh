#!/bin/bash
# Test: if five elif
if false; then echo 1; elif false; then echo 2; elif false; then echo 3; elif false; then echo 4; elif false; then echo 5; elif true; then echo 6; else echo 7; fi
