#!/bin/bash
# Test: until with variables
x=start; until [ $x = end ]; do echo $x; x=end; done
