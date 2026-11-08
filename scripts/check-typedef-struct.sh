#!/bin/bash

echo "=========================================="
echo "VERIFICATION: typedef struct (name.struct)"
echo "=========================================="
echo ""

found_issues=0
declare -a issues_list

while IFS= read -r file; do
    violations=$(grep -n "^typedef struct" "$file" 2>/dev/null)
    
    if [ -n "$violations" ]; then
        found_issues=$((found_issues + 1))
        issues_list+=("$file")
        echo "⚠️  $file:"
        echo "$violations"
        echo ""
    fi
done < <(find src -type f \( -name "*.c" -o -name "*.h" \))

echo ""
if [ $found_issues -eq 0 ]; then
    echo "✓ Aucun typedef struct detecte"
    exit 0
else
    echo "⚠️  $found_issues fichier(s) avec typedef struct"
    echo ""
    echo "Fichiers a corriger:"
    for issue in "${issues_list[@]}"; do
        echo "  - $issue"
    done
    exit 1
fi
