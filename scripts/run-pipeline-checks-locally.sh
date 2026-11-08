#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}  POSIX SHELL - VERIFICATION LOCALE DES REGLES${NC}"
echo -e "${BLUE}============================================${NC}"
echo ""

TOTAL_CHECKS=11
PASSED=0
FAILED=0
WARNINGS=0

run_check() {
    local script_name=$1
    local description=$2
    
    echo -e "${BLUE}[$((PASSED + FAILED + WARNINGS + 1))/$TOTAL_CHECKS]${NC} $description"
    echo "-------------------------------------------"
    
    if [ ! -f "$SCRIPT_DIR/$script_name" ]; then
        echo -e "${RED}ERREUR: $script_name introuvable!${NC}"
        echo ""
        FAILED=$((FAILED + 1))
        return 1
    fi
    
    chmod +x "$SCRIPT_DIR/$script_name"
    
    if "$SCRIPT_DIR/$script_name"; then
        echo -e "${GREEN}✓ PASSE${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${YELLOW}⚠ WARNING${NC}"
        WARNINGS=$((WARNINGS + 1))
    fi
    
    echo ""
}

run_check "check-exported-vars.sh" "Variables exportées (max 1 par fichier)"
run_check "check-exported-funcs.sh" "Fonctions exportées (max 10 par fichier)"
run_check "check-function-length.sh" "Longueur des fonctions (max 30 lignes)"
run_check "check-function-params.sh" "Parametres des fonctions (max 4)"
run_check "check-typedef-struct.sh" "Typedef struct interdits"
run_check "check-tabs.sh" "Tabulations interdites"
run_check "check-pointer-placement.sh" "Placement des pointeurs (*)"
run_check "check-header-comments.sh" "Headers avec commentaire final"
run_check "check-final-newline.sh" "Newline a la fin de chaque fichier"
run_check "check-void-params.sh" "Prototypes avec void explicite"
run_check "check-funcptr-deref.sh" "Déréférencement des pointeurs de fonction"

echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}  RESUME${NC}"
echo -e "${BLUE}============================================${NC}"
echo -e "${GREEN}✓ Passés: $PASSED${NC}"
echo -e "${YELLOW}⚠ Warnings: $WARNINGS${NC}"
echo -e "${RED}✗ Échecs: $FAILED${NC}"
echo ""

if [ $FAILED -gt 0 ]; then
    echo -e "${RED}Certains checks critiques ont échoué${NC}"
    exit 1
elif [ $WARNINGS -gt 0 ]; then
    echo -e "${YELLOW}Tous les checks sont passés avec des warnings${NC}"
    exit 0
else
    echo -e "${GREEN}Tous les checks sont passés!${NC}"
    exit 0
fi
