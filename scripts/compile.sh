#!/bin/bash

set -e

echo "=========================================="
echo "COMPILATION DU PROJET POSIX SHELL"
echo "=========================================="
echo

echo "[1/6] Nettoyage..."
make maintainer-clean 2>/dev/null || true
make distclean 2>/dev/null || true
make clean 2>/dev/null || true
rm -f config.log config.status 2>/dev/null || true
echo "✓ Nettoyage terminé"
echo

echo "[2/6] Génération des fichiers autotools..."
autoreconf --install
if [ $? -ne 0 ]; then
    echo "❌ Erreur lors de autoreconf"
    exit 1
fi
echo "✓ Autotools généré"
echo

echo "[3/6] Configuration..."
./configure
if [ $? -ne 0 ]; then
    echo "❌ Erreur lors de configure"
    exit 1
fi
echo "✓ Configuration terminée"
echo

echo "[4/6] Compilation..."
make
if [ $? -ne 0 ]; then
    echo "❌ Erreur lors de make"
    exit 1
fi
echo "✓ Compilation terminée"
echo

echo "[5/6] Vérification du binaire..."
if [ -f "src/posix-shell" ]; then
    echo "✓ Binaire créé : src/posix-shell"
    ls -lh src/posix-shell
else
    echo "❌ Binaire non trouvé"
    exit 1
fi
echo

echo "[6/6] Test rapide..."
echo "test" | ./src/posix-shell -c "echo hello"
if [ $? -eq 0 ]; then
    echo "✓ Test rapide réussi"
else
    echo "⚠ Test rapide échoué"
fi
echo

echo "=========================================="
echo "✅ COMPILATION TERMINÉE AVEC SUCCÈS"
echo "=========================================="
echo
echo "Pour lancer les tests :"
echo "  BIN_PATH=\"\$(pwd)/src/posix-shell\" ./tests/run_tests.sh"
echo
echo "Pour utiliser le shell :"
echo "  ./src/posix-shell"
echo
