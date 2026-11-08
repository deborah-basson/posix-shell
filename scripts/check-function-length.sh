#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "Verification: 30 lines max per function"
echo "========================================"
echo ""

declare -a all_violations

for file in $(find src -name "*.c" -type f 2>/dev/null | sort); do
    while IFS= read -r violation; do
        if [ -n "$violation" ]; then
            all_violations+=("$violation")
        fi
    done < <(awk '
    BEGIN {
        in_func = 0
        line_count = 0
        func_name = ""
        brace_depth = 0
        collecting_signature = 0
        start_line = 0
    }
    
    /^[a-zA-Z_].*[a-zA-Z_0-9]\(/ && !in_func && $0 !~ /;[\s]*$/ {
        collecting_signature = 1
        func_name = $0
        start_line = NR
        
        if ($0 ~ /\{/) {
            in_func = 1
            collecting_signature = 0
            line_count = 1
            brace_depth = gsub(/\{/, "{", $0)
            brace_depth -= gsub(/\}/, "}", $0)
        }
        next
    }
    
    collecting_signature {
        func_name = func_name " " $0
        if ($0 ~ /\{/) {
            in_func = 1
            collecting_signature = 0
            line_count = 1
            brace_depth = gsub(/\{/, "{", $0)
            brace_depth -= gsub(/\}/, "}", $0)
        }
        next
    }
    
    in_func {
        line_count++
        
        open_braces = gsub(/\{/, "{", $0)
        close_braces = gsub(/\}/, "}", $0)
        brace_depth += open_braces
        brace_depth -= close_braces
        
        if (brace_depth == 0) {
            if (line_count > 30) {
                clean_name = func_name
                gsub(/^[ \t]+/, "", clean_name)
                gsub(/[ \t]+/, " ", clean_name)
                sub(/[\s]*\{.*/, "", clean_name)
                gsub(/[\n\r]/, " ", clean_name)
                print FILENAME ":" clean_name " -> " line_count " lines"
            }
            in_func = 0
            collecting_signature = 0
            line_count = 0
            func_name = ""
            brace_depth = 0
        }
    }
    ' "$file")
done

if [ ${#all_violations[@]} -gt 0 ]; then
    echo -e "${YELLOW}Found ${#all_violations[@]} function(s) exceeding 30 lines:${NC}"
    echo ""
    for violation in "${all_violations[@]}"; do
        echo "$violation"
    done
    echo ""
    echo -e "${RED}FAIL${NC}: Some functions exceed 30 lines"
    exit 1
else
    echo -e "${GREEN}OK${NC}: All functions respect the limit"
    exit 0
fi