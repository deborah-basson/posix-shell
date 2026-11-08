#!/bin/bash
# Test: function in pipeline
gen() { echo data; }; proc() { cat; }; gen | proc
