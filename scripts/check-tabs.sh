#!/bin/bash

echo "=========================================="
echo "VERIFICATION: Tabulations (braces.indent)"
echo "=========================================="
echo ""

found_issues=0
declare -a issues_list

while IFS= read -r file; do
    violations=$(grep -n $'^\t' "$file" 2>/dev/null)
    
    if [ -n "$violations" ]; then
        found_issues=$((found_issues + 1))
        issues_list+=("$file")
        echo "⚠️  $file:"
        echo "$violations" | head -5
        count=$(echo "$violations" | wc -l)
        if [ "$count" -gt 5 ]; then
            echo "... et $((count - 5)) autre(s) ligne(s)"
        fi
        echo ""
    fi
done < <(find src -type f \( -name "*.c" -o -name "*.h" \))

echo ""
if [ $found_issues -eq 0 ]; then
    echo "✓ Aucune tabulation detectee"
    exit 0
else
    echo "⚠️  $found_issues fichier(s) avec tabulations"
    echo ""
    echo "Fichiers a corriger:"
    for issue in "${issues_list[@]}"; do
        echo "  - $issue"
    done
    exit 1
fi
