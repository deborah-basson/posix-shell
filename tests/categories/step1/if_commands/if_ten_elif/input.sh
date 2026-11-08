#!/bin/bash
# Test: if ten elif
if false; then echo 1; elif false; then echo 2; elif false; then echo 3; elif false; then echo 4; elif false; then echo 5; elif false; then echo 6; elif false; then echo 7; elif false; then echo 8; elif false; then echo 9; elif false; then echo 10; elif true; then echo 11; else echo 12; fi
