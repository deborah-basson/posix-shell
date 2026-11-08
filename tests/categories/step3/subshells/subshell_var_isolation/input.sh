#!/bin/bash
# Test: subshell var isolation
a=outer; (a=inner; echo $a); echo $a
