#!/bin/bash
# Test: elif sans condition (stderr expected)
if false; then echo no; elif; then echo ok; fi
