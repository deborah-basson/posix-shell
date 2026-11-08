#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "Verification: empty parenthesis must specify void"
echo "=================================================="

VIOLATIONS=0
declare -a violations_list

for file in $(find src -name "*.h" -type f); do
    while IFS= read -r line; do
        if echo "$line" | grep -qE "^[a-zA-Z_].*\([[:space:]]*\)" && \
           ! echo "$line" | grep -q "void" && \
           ! echo "$line" | grep -q "//" && \
           ! echo "$line" | grep -q "#define"; then
            VIOLATIONS=$((VIOLATIONS + 1))
            violations_list+=("$file:$line")
        fi
    done < "$file"
done

if [ ${#violations_list[@]} -gt 0 ]; then
    for violation in "${violations_list[@]}"; do
        file="${violation%%:*}"
        line="${violation#*:}"
        echo -e "${RED}FAIL${NC}: $file"
        echo "  $line"
    done
fi

echo ""
if [ $VIOLATIONS -eq 0 ]; then
    echo -e "${GREEN}OK${NC}: All function prototypes use void or have parameters"
    exit 0
else
    echo -e "${RED}FAIL${NC}: $VIOLATIONS violation(s) found"
    exit 1
fi