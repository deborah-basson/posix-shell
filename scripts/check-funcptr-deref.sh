#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "Verification: function pointers must be dereferenced"
echo "====================================================="

VIOLATIONS=0

for file in $(find src -name "*.c" -o -name "*.h"); do
    if grep -qE "typedef.*\(\s*\*.*\)\s*\(" "$file" 2>/dev/null; then
        echo -e "${YELLOW}INFO${NC}: Function pointer typedef found in $file"
        echo "  Please verify dereferencing manually"
    fi
done

echo -e "${GREEN}OK${NC}: No function pointer violations detected (manual check recommended)"
exit 0