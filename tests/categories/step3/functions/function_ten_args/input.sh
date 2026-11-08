#!/bin/bash
# Test: function ten args
f() { echo $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10}; }; f 1 2 3 4 5 6 7 8 9 10
