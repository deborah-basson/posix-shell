#!/bin/bash
# Test: complex chain
true && echo a || echo b; false || echo c && echo d
