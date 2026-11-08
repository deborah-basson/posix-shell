#!/bin/bash
# Test: redirect before command
>out.txt echo test; cat out.txt; rm out.txt
