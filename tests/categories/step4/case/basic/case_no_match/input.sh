#!/bin/bash
run_test "case no match" "case xyz in a) echo A;; b) echo B;; esac" 0 ""