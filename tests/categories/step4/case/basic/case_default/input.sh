#!/bin/bash
#cas default

run_test "case default" "case xyz in a) echo A;; *) echo default;; esac" 0 "default"