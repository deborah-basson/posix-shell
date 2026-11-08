#!/bin/bash
# Test: IFS=:, split sur deux-points et virgule
IFS=:,; var="a:b,c"; echo $var
