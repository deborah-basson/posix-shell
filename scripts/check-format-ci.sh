#!/bin/bash

set -e

echo "Verification du format du code avec clang-format..."
echo ""

if [ ! -f ".clang-format" ]; then
    echo "ERREUR: Le fichier .clang-format est introuvable!"
    exit 1
fi

echo "Fichier .clang-format trouve"
echo ""

FILES=$(find src tests -type f \( -name "*.c" -o -name "*.h" \) 2>/dev/null)

if [ -z "$FILES" ]; then
    echo "Aucun fichier .c ou .h trouve"
    exit 0
fi

TOTAL_FILES=$(echo "$FILES" | wc -l)
echo "$TOTAL_FILES fichier(s) a verifier"
echo ""

FORMAT_ERRORS=0
CHECKED_FILES=0

> /tmp/bad_files.txt

for file in $FILES; do
    CHECKED_FILES=$((CHECKED_FILES + 1))
    
    if ! clang-format --style=file "$file" | diff -qu "$file" - > /dev/null 2>&1; then
        echo "[FAIL] [$CHECKED_FILES/$TOTAL_FILES] $file"
        echo "$file" >> /tmp/bad_files.txt
        FORMAT_ERRORS=$((FORMAT_ERRORS + 1))
    else
        echo "[OK] [$CHECKED_FILES/$TOTAL_FILES] $file"
    fi
done

echo ""
echo "=========================================="
echo "RESUME"
echo "=========================================="
echo "Fichiers verifies: $CHECKED_FILES"
echo "Fichiers conformes: $((CHECKED_FILES - FORMAT_ERRORS))"
echo "Fichiers non conformes: $FORMAT_ERRORS"
echo ""

if [ $FORMAT_ERRORS -eq 0 ]; then
    echo "SUCCESS: Tous les fichiers sont correctement formates!"
    exit 0
else
    echo "ECHEC: $FORMAT_ERRORS fichier(s) non conforme(s)"
    echo ""
    echo "Liste des fichiers a corriger:"
    cat /tmp/bad_files.txt | sed 's/^/  - /'
    echo ""
    echo "Executez './scripts/clang-format.sh' pour corriger"
    exit 1
fi