#!/bin/bash

echo "=========================================="
echo "VERIFICATION DES VARIABLES EXPORTEES"
echo "=========================================="
echo ""

found_issues=0
declare -a issues_list

while IFS= read -r file; do
    if [[ "$file" == *.h ]]; then
        count=$(grep -E '^\s*extern\s+' "$file" | grep -v '(' | wc -l)
    else
        count=$(grep -E '^[a-zA-Z_][a-zA-Z0-9_]*\s+(\*+\s*)*[a-zA-Z_][a-zA-Z0-9_]*\s*=' "$file" | grep -v 'static' | wc -l)
    fi
    
    if [ "$count" -gt 1 ]; then
        found_issues=$((found_issues + 1))
        issues_list+=("$file:$count")
    fi
done < <(find src -type f \( -name "*.c" -o -name "*.h" \))

if [ $found_issues -gt 0 ]; then
    echo "Fichiers avec violations:"
    echo ""
    for issue in "${issues_list[@]}"; do
        file="${issue%:*}"
        count="${issue#*:}"
        echo "⚠️  $file: $count variables exportées"
        
        if [[ "$file" == *.h ]]; then
            grep -n -E '^\s*extern\s+' "$file" | grep -v '('
        else
            grep -n -E '^[a-zA-Z_][a-zA-Z0-9_]*\s+(\*+\s*)*[a-zA-Z_][a-zA-Z0-9_]*\s*=' "$file" | grep -v 'static'
        fi
        echo ""
    done
fi

echo ""
if [ $found_issues -eq 0 ]; then
    echo "✓ Aucun problème détecté"
    exit 0
else
    echo "⚠️  $found_issues fichier(s) avec plus d'une variable exportée"
    echo ""
    echo "Fichiers à corriger:"
    for issue in "${issues_list[@]}"; do
        echo "  - ${issue%:*}"
    done
    exit 1
fi