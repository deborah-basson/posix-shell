#!/bin/bash
# Test: redirect output tmp
echo test >/tmp/posix-shell_test.txt; cat /tmp/posix-shell_test.txt; rm /tmp/posix-shell_test.txt
