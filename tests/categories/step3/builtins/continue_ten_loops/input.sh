#!/bin/bash
# Test: continue ten loops
a=0; while [ $a -lt 1 ]; do a=1; b=0; while [ $b -lt 1 ]; do b=1; c=0; while [ $c -lt 1 ]; do c=1; d=0; while [ $d -lt 1 ]; do d=1; e=0; while [ $e -lt 1 ]; do e=1; f=0; while [ $f -lt 1 ]; do f=1; g=0; while [ $g -lt 1 ]; do g=1; h=0; while [ $h -lt 1 ]; do h=1; i=0; while [ $i -lt 1 ]; do i=1; j=0; while [ $j -lt 1 ]; do j=1; echo deep; continue 10; done; done; done; done; done; done; done; done; done; done; echo ok
