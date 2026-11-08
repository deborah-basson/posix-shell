#!/bin/bash
# Test: redirect stdin stdout
echo test >in.txt; cat <in.txt >out.txt; cat out.txt; rm in.txt out.txt
