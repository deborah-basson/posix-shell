#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  POSIX SHELL - COVERAGE REPORT${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

cleanup() {
    echo -e "${YELLOW}Nettoyage...${NC}"
    rm -f posix-shell
    find . -name "*.gcda" -delete 2>/dev/null
    find . -name "*.gcno" -delete 2>/dev/null
    find . -name "*.gcov" -delete 2>/dev/null
    rm -f coverage.txt coverage.html coverage.*.html *.txt *.css
    rm -f '$(echo file.txt)'
    echo -e "${GREEN}✓ Nettoyage terminé${NC}"
}

compile_with_coverage() {
    echo -e "${YELLOW}Compilation avec --coverage...${NC}"
    
    if [ ! -f "./configure" ]; then
        echo -e "${CYAN}Génération du script configure...${NC}"
        autoreconf -i > /dev/null 2>&1
        if [ $? -ne 0 ]; then
            echo -e "${RED}✗ Erreur lors de autoreconf${NC}"
            return 1
        fi
    fi
    
    make clean > /dev/null 2>&1
    
    echo -e "${CYAN}Configuration avec --coverage...${NC}"
    ./configure CFLAGS="--coverage -O0 -g" LDFLAGS="--coverage" > /dev/null 2>&1
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}✗ Erreur de configuration${NC}"
        return 1
    fi
    
    echo -e "${CYAN}Compilation...${NC}"
    make -j4 > /dev/null 2>&1
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}✗ Erreur de compilation${NC}"
        return 1
    fi
    
    # Copier le binaire à la racine
    if [ -f "src/posix-shell" ]; then
        cp src/posix-shell ./posix-shell
        echo -e "${GREEN}✓ Compilation réussie${NC}"
        return 0
    else
        echo -e "${RED}✗ Binaire src/posix-shell introuvable${NC}"
        return 1
    fi
}

run_functional_tests() {
    echo -e "${YELLOW}Exécution des tests fonctionnels...${NC}"
    
    if [ ! -f "./posix-shell" ]; then
        echo -e "${RED}✗ Binaire ./posix-shell introuvable${NC}"
        return 1
    fi
    
    if [ -d "tests" ] && [ -f "tests/run_tests.sh" ]; then
        echo -e "${CYAN}Lancement de tests/run_tests.sh...${NC}"
        BIN_PATH="$PWD/posix-shell" ./tests/run_tests.sh > /dev/null 2>&1
        echo -e "${GREEN}✓ Tests fonctionnels exécutés${NC}"
    else
        echo -e "${YELLOW}⚠ Fichier tests/run_tests.sh introuvable, ignoré${NC}"
    fi
    
    return 0
}

run_unit_tests() {
    echo -e "${YELLOW}Exécution des tests unitaires...${NC}"
    
    if [ ! -d "tests/unit" ]; then
        echo -e "${YELLOW}⚠ Pas de dossier tests/unit, tests unitaires ignorés${NC}"
        return 0
    fi
    
    echo -e "${CYAN}Exécution des tests unitaires...${NC}"
    
    make -C tests/unit check > /dev/null 2>&1
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Tous les tests unitaires passent${NC}"
    else
        echo -e "${YELLOW}⚠ Certains tests unitaires ont échoué (coverage généré quand même)${NC}"
    fi
    
    return 0
}

generate_coverage_report() {
    echo -e "${YELLOW}Génération du rapport de coverage...${NC}"
    
    if ! command -v gcovr &> /dev/null; then
        echo -e "${RED}✗ gcovr non installé${NC}"
        echo -e "${YELLOW}  Pour l'installer: pip3 install gcovr${NC}"
        echo -e "${YELLOW}  ou: sudo apt install gcovr${NC}"
        return 1
    fi
    
    gcda_count=$(find . -name "*.gcda" | wc -l)
    if [ $gcda_count -eq 0 ]; then
        echo -e "${RED}✗ Aucun fichier .gcda trouvé${NC}"
        echo -e "${YELLOW}  Les tests n'ont peut-être pas été exécutés${NC}"
        return 1
    fi
    
    echo -e "${CYAN}Fichiers de coverage trouvés: $gcda_count${NC}"
    
    gcovr --txt coverage.txt \
          --html coverage.html \
          --html-details \
          -r . \
          --exclude-unreachable-branches \
          --exclude-throw-branches \
          --exclude 'tests/*' \
          --gcov-ignore-errors=no_working_dir_found \
          2>/dev/null
    
    if [ -f "coverage.txt" ]; then
        echo -e "${GREEN}✓ Rapport généré${NC}"
        return 0
    else
        echo -e "${RED}✗ Fichier coverage.txt non créé${NC}"
        return 1
    fi
}

display_report() {
    echo ""
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  RAPPORT DE COVERAGE${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
    
    if [ ! -f "coverage.txt" ]; then
        echo -e "${RED}✗ Fichier coverage.txt introuvable${NC}"
        return 1
    fi
    
    cat coverage.txt
    
    echo ""
    echo -e "${BLUE}========================================${NC}"
    
    overall=$(grep "TOTAL" coverage.txt | awk '{print $4}' | sed 's/%//')
    
    if [ -n "$overall" ]; then
        if [ "$overall" -ge 80 ]; then
            echo -e "${GREEN}✓ Coverage: ${overall}% (objectif atteint)${NC}"
        else
            echo -e "${YELLOW}⚠ Coverage: ${overall}% (objectif: 80%)${NC}"
            
            total_lines=$(grep "TOTAL" coverage.txt | awk '{print $2}')
            covered_lines=$(grep "TOTAL" coverage.txt | awk '{print $3}')
            target_lines=$((total_lines * 80 / 100))
            missing_lines=$((target_lines - covered_lines))
            
            if [ $missing_lines -gt 0 ]; then
                echo -e "${YELLOW}  Il manque environ ${missing_lines} lignes pour atteindre 80%${NC}"
            fi
        fi
    fi
    
    if [ -f "coverage.html" ]; then
        echo -e "${CYAN}📊 Rapport HTML: coverage.html${NC}"
        echo -e "${CYAN}   Ouvrez-le dans votre navigateur pour l'analyse détaillée${NC}"
    fi
    
    echo -e "${BLUE}========================================${NC}"
}

analyze_low_coverage() {
    echo ""
    echo -e "${CYAN}╔═══════════════════════════════════╗${NC}"
    echo -e "${CYAN}  ANALYSE DES FICHIERS À AMÉLIORER${NC}"
    echo -e "${CYAN}╚═══════════════════════════════════╝${NC}"
    echo ""
    
    if [ ! -f "coverage.txt" ]; then
        return 1
    fi
    
    critical_count=0
    medium_count=0
    
    echo -e "${RED}Fichiers critiques (< 50%):${NC}"
    while IFS= read -r line; do
        if [[ "$line" =~ ^src/ ]]; then
            file=$(echo "$line" | awk '{print $1}')
            cover=$(echo "$line" | awk '{print $4}' | sed 's/%//')
            lines=$(echo "$line" | awk '{print $2}')
            
            if [ -n "$cover" ] && [ "$cover" -lt 50 ] 2>/dev/null; then
                critical_count=$((critical_count + 1))
                printf "${RED}  %2d. %-50s %3s%% (%s lignes)${NC}\n" "$critical_count" "$file" "$cover" "$lines"
            fi
        fi
    done < coverage.txt
    
    if [ $critical_count -eq 0 ]; then
        echo -e "${GREEN}  ✓ Aucun fichier critique${NC}"
    fi
    
    echo ""
    echo -e "${YELLOW}Fichiers moyens (50-79%):${NC}"
    while IFS= read -r line; do
        if [[ "$line" =~ ^src/ ]]; then
            file=$(echo "$line" | awk '{print $1}')
            cover=$(echo "$line" | awk '{print $4}' | sed 's/%//')
            lines=$(echo "$line" | awk '{print $2}')
            
            if [ -n "$cover" ] && [ "$cover" -ge 50 ] && [ "$cover" -lt 80 ] 2>/dev/null; then
                medium_count=$((medium_count + 1))
                printf "${YELLOW}  %2d. %-50s %3s%% (%s lignes)${NC}\n" "$medium_count" "$file" "$cover" "$lines"
            fi
        fi
    done < coverage.txt
    
    if [ $medium_count -eq 0 ]; then
        echo -e "${GREEN}  ✓ Aucun fichier moyen${NC}"
    fi
    
    echo ""
    echo -e "${BLUE}╔═══════════════════════════════════╗${NC}"
    echo -e "${CYAN}Résumé:${NC}"
    echo -e "  ${RED}Critiques (< 50%):${NC} $critical_count fichier(s)"
    echo -e "  ${YELLOW}Moyens (50-79%):${NC} $medium_count fichier(s)"
    total_to_improve=$((critical_count + medium_count))
    echo -e "  ${CYAN}Total à améliorer:${NC} $total_to_improve fichier(s)"
    echo -e "${BLUE}╚═══════════════════════════════════╝${NC}"
}

case "${1:-}" in
    clean)
        cleanup
        ;;
    *)
        cleanup
        echo ""
        
        compile_with_coverage || exit 1
        echo ""
        
        run_functional_tests
        echo ""
        
        run_unit_tests
        echo ""
        
        generate_coverage_report || exit 1
        
        display_report
        
        analyze_low_coverage
        
        echo ""
        echo -e "${GREEN}✓ Analyse terminée${NC}"
        echo ""
        ;;
esac
