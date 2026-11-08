#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "Verification: .h files must end with a comment"
echo "==============================================="

VIOLATIONS=0
declare -a violations_list

for file in $(find src -name "*.h" -type f); do
    last_line=$(tail -1 "$file")
    
    if [[ ! "$last_line" =~ ^[[:space:]]*// ]] && [[ ! "$last_line" =~ \*/ ]]; then
        VIOLATIONS=$((VIOLATIONS + 1))
        violations_list+=("$file")
    fi
done

if [ ${#violations_list[@]} -gt 0 ]; then
    for file in "${violations_list[@]}"; do
        last_line=$(tail -1 "$file")
        echo -e "${RED}FAIL${NC}: $file does not end with a comment"
        echo "  Last line: $last_line"
    done
fi

echo ""
if [ $VIOLATIONS -eq 0 ]; then
    echo -e "${GREEN}OK${NC}: All header files end with a comment"
    exit 0
else
    echo -e "${RED}FAIL${NC}: $VIOLATIONS file(s) violate the rule"
    exit 1
fi