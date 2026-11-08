#!/bin/bash
# Test: function complex realistic
process() { if [ $# -eq 0 ]; then echo 'no args'; return 1; fi; for arg in \"$@\"; do echo \"Processing: $arg\"; done; return 0; }; process file1 file2 file3
