#!/bin/bash
run_test "case multiple" "case 2 in 1) echo one;; 2) echo two;; 3) echo three;; esac" 0 "two"