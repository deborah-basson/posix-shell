#!/bin/bash
# Test: status in while
i=0; while [ $i -lt 1 ]; do true; echo $?; i=1; done
