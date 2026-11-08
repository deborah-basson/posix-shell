#!/bin/bash
set -e

echo "Formatage avec votre version locale de clang-format..."
echo ""
clang-format --version
echo ""

FILES=$(find src tests -type f \( -name "*.c" -o -name "*.h" \) 2>/dev/null)

if [ -z "$FILES" ]; then
    echo "Aucun fichier trouve"
    exit 0
fi

TOTAL=$(echo "$FILES" | wc -l)
echo "Fichiers a traiter: $TOTAL"
echo ""

echo "Etape 1: Application de clang-format"
FORMATTED=0
for file in $FILES; do
    BEFORE=$(md5sum "$file" | cut -d' ' -f1)
    clang-format -i --style=file "$file"
    AFTER=$(md5sum "$file" | cut -d' ' -f1)
    
    if [ "$BEFORE" != "$AFTER" ]; then
        FORMATTED=$((FORMATTED + 1))
        echo "Formate: $file"
    fi
done

echo ""
echo "Fichiers formattés: $FORMATTED"
echo ""

echo "Etape 2: Correction des parentheses de pointeurs"
FIXED=0
for file in $FILES; do
    BEFORE=$(md5sum "$file" | cut -d' ' -f1)
    
    sed -i 's/\(int\|char\|void\|float\|double\|long\|short\|size_t\|pid_t\|unsigned\|signed\) (\*\([^)]*\))(/\1(*\2)(/g' "$file"
    
    sed -i 's/^\([[:space:]]*\)\(int\|char\|void\|float\|double\|long\|short\|size_t\|pid_t\|unsigned\|signed\) (\*\([^)]*\))\[/\1\2(*\3)[/g' "$file"
    
    sed -i 's/^\([[:space:]]*\)\(int\|char\|void\|float\|double\|long\|short\|size_t\|pid_t\|unsigned\|signed\)(\*\([^)]*\))\[/\1\2 (*\3)[/g' "$file"
    
    AFTER=$(md5sum "$file" | cut -d' ' -f1)
    
    if [ "$BEFORE" != "$AFTER" ]; then
        FIXED=$((FIXED + 1))
        echo "Corrige: $file"
    fi
done

echo ""
echo "Fichiers corriges: $FIXED"
echo ""

echo "=========================================="
echo "RESULTAT FINAL"
echo "=========================================="
echo "Fichiers traites: $TOTAL"
echo "Fichiers formattés: $FORMATTED"
echo "Fichiers corriges (pointeurs): $FIXED"
echo ""
echo "SUCCESS: Formatage termine"
echo ""
echo "Note: Pointeurs de fonction ET variables locales sont corriges"