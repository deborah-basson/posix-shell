#!/bin/bash

echo "=========================================="
echo "VERIFICATION: Placement des * (decl.point)"
echo "=========================================="
echo ""

found_issues=0
declare -a issues_list

while IFS= read -r file; do
    violations=$(grep -n -E '^\s*(const\s+)?(struct\s+\w+|int|char|void|float|double|long|short|size_t|pid_t|unsigned|signed|FILE)\*+\s+\w+' "$file" 2>/dev/null | grep -v "^\s*//" | grep -v "^\s*/\*")
    
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
    echo "✓ Tous les pointeurs sont correctement places"
    echo ""
    echo "Rappel EPITA: Le * doit etre colle au nom de variable"
    echo "Exemple CORRECT: 'char *ptr' ou 'struct token *tok'"
    echo "Exemple INCORRECT: 'char* ptr' ou 'char*ptr'"
    exit 0
else
    echo "⚠️  $found_issues fichier(s) avec pointeurs mal places"
    echo ""
    echo "Rappel EPITA: Le * doit etre colle au nom de variable, pas au type"
    echo "Exemple CORRECT: 'char *ptr' (espace entre type et *)"
    echo "Exemple INCORRECT: 'char* ptr' (espace entre * et nom) ou 'char*ptr' (pas d'espace)"
    echo ""
    echo "Fichiers a corriger:"
    for issue in "${issues_list[@]}"; do
        echo "  - $issue"
    done
    exit 1
fi