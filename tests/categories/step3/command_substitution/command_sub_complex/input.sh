#!/bin/bash
# Test: command sub complex
x=$(for i in 1 2 3; do echo $i; done); echo $x
