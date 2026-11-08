#!/bin/bash
# Test: for with || operator
for i in a b; do false || echo $i; done
