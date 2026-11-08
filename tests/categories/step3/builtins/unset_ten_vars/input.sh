#!/bin/bash
# Test: unset ten vars
V1=1;V2=2;V3=3;V4=4;V5=5;V6=6;V7=7;V8=8;V9=9;V10=10;unset V1 V2 V3 V4 V5 V6 V7 V8 V9 V10;echo $V1$V5$V10
