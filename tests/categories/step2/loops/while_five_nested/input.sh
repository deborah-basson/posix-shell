#!/bin/bash
# Test: while five nested
a=0; while [ $a -lt 1 ]; do b=0; while [ $b -lt 1 ]; do c=0; while [ $c -lt 1 ]; do d=0; while [ $d -lt 1 ]; do e=0; while [ $e -lt 1 ]; do echo deep; e=1; done; d=1; done; c=1; done; b=1; done; a=1; done
