#!/bin/bash
# Test: block redir output
{ echo line1; echo line2; } > /tmp/posix-shell_test_block.txt; cat /tmp/posix-shell_test_block.txt; rm /tmp/posix-shell_test_block.txt
