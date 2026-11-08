#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "Verification: 10 exported functions max per file"
echo "================================================="

VIOLATIONS=0
declare -a violations_list

for file in $(find src -name "*.c" -type f); do
    count=$(grep -E "^[a-zA-Z_][a-zA-Z0-9_]*\s+[a-zA-Z_][a-zA-Z0-9_]*\s*\(" "$file" | grep -v "^static" | wc -l)
    
    if [ "$count" -gt 10 ]; then
        VIOLATIONS=$((VIOLATIONS + 1))
        violations_list+=("$file:$count")
    fi
done

if [ ${#violations_list[@]} -gt 0 ]; then
    for violation in "${violations_list[@]}"; do
        file="${violation%:*}"
        count="${violation#*:}"
        echo -e "${RED}FAIL${NC}: $file has $count exported functions (max: 10)"
    done
fi

echo ""
if [ $VIOLATIONS -eq 0 ]; then
    echo -e "${GREEN}OK${NC}: All files respect the limit"
    exit 0
else
    echo -e "${RED}FAIL${NC}: $VIOLATIONS file(s) violate the rule"
    exit 1
fi