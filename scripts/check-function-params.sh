#!/bin/bash

echo "=========================================="
echo "VERIFICATION: Parametres de fonction (fun.arg.count)"
echo "=========================================="
echo ""

found_issues=0
declare -a violations_list

for file in $(find src -type f \( -name "*.c" -o -name "*.h" \) | sort); do
    while IFS= read -r violation; do
        if [ -n "$violation" ]; then
            found_issues=$((found_issues + 1))
            violations_list+=("$violation")
        fi
    done < <(awk '
        BEGIN {
            sig = ""
            collecting = 0
            start = 0
        }
        
        /^[a-zA-Z_]/ && !collecting && $0 !~ /^#/ {
            sig = $0
            start = NR
            collecting = 1
        }
        
        collecting && NR > start {
            sig = sig " " $0
            
            if ($0 ~ /\)/) {
                gsub(/[\n\r\t]+/, " ", sig)
                gsub(/  +/, " ", sig)
                
                if (sig ~ /;[ ]*$/) {
                    collecting = 0
                    sig = ""
                    next
                }
                
                if (match(sig, /\([^)]*\)/)) {
                    params = substr(sig, RSTART+1, RLENGTH-2)
                    gsub(/^[ \t]+/, "", params)
                    gsub(/[ \t]+$/, "", params)
                    
                    if (params != "" && params != "void") {
                        n = split(params, arr, /,/)
                        if (n > 4) {
                            print FILENAME ":" start ":" sig " -> " n " parametres"
                        }
                    }
                }
                
                collecting = 0
                sig = ""
            }
        }
    ' "$file")
done

if [ ${#violations_list[@]} -gt 0 ]; then
    echo "Fonctions avec plus de 4 parametres:"
    echo ""
    for violation in "${violations_list[@]}"; do
        echo "$violation"
    done
fi

echo ""
if [ $found_issues -eq 0 ]; then
    echo "✓ Toutes les fonctions ont au maximum 4 parametres"
    exit 0
else
    echo "⚠️  $found_issues fonction(s) depassent la limite de 4 parametres"
    echo ""
    echo "Fichiers a corriger:"
    for violation in "${violations_list[@]}"; do
        file=$(echo "$violation" | cut -d: -f1)
        echo "  - $file"
    done | sort -u
    exit 1
fi