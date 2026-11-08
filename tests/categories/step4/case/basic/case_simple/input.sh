#!/bin/bash

#Test case simple
run_test "case simple match" "case hello in hello) echo matched;; esac" 0 "matched"