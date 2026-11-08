#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "Verification: files must end with a newline"
echo "============================================"

VIOLATIONS=0
declare -a violations_list

for file in $(find src -type f \( -name "*.c" -o -name "*.h" \)); do
    if [ ! -s "$file" ]; then
        continue
    fi
    
    last_char=$(tail -c 1 "$file")
    
    if [ -n "$last_char" ]; then
        VIOLATIONS=$((VIOLATIONS + 1))
        violations_list+=("$file")
    fi
done

if [ ${#violations_list[@]} -gt 0 ]; then
    for file in "${violations_list[@]}"; do
        echo -e "${RED}FAIL${NC}: $file does not end with a newline"
    done
fi

echo ""
if [ $VIOLATIONS -eq 0 ]; then
    echo -e "${GREEN}OK${NC}: All files end with a newline"
    exit 0
else
    echo -e "${RED}FAIL${NC}: $VIOLATIONS file(s) do not end with a newline"
    exit 1
fi