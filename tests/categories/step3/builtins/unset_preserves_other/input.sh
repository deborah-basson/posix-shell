#!/bin/bash
# Test: unset preserves other
A=1; B=2; unset A; echo $B
