#!/bin/bash
# Test: redirect with negation
! false && echo ok >neg.txt; cat neg.txt; rm neg.txt
