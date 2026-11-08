#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

init_test_counters() {
    TOTAL_TESTS=0
    PASSED_TESTS=0
    FAILED_TESTS=0
}

increment_total() {
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
}

increment_passed() {
    PASSED_TESTS=$((PASSED_TESTS + 1))
}

increment_failed() {
    FAILED_TESTS=$((FAILED_TESTS + 1))
}

get_percentage() {
    if [ "$TOTAL_TESTS" -eq 0 ]; then
        echo "0"
    else
        echo $((PASSED_TESTS * 100 / TOTAL_TESTS))
    fi
}

print_category_header() {
    local category="$1"
    echo ""
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  Testing: $category${NC}"
    echo -e "${BLUE}========================================${NC}"
}

print_test_result() {
    local test_name="$1"
    local status="$2"
    local details="$3"
    
    if [ "$status" = "PASS" ]; then
        echo -e "${GREEN}✓${NC} $test_name"
    else
        echo -e "${RED}✗${NC} $test_name"
        if [ -n "$details" ]; then
            echo -e "  ${YELLOW}→${NC} $details"
        fi
    fi
}

print_category_summary() {
    local category="$1"
    local passed="$2"
    local total="$3"
    local percentage=$((passed * 100 / total))
    
    echo ""
    if [ "$passed" -eq "$total" ]; then
        echo -e "${GREEN}[$category] $passed/$total ($percentage%)${NC}"
    else
        echo -e "${YELLOW}[$category] $passed/$total ($percentage%)${NC}"
    fi
}

print_global_summary() {
    local percentage=$(get_percentage)
    
    echo ""
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  Global Results${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo -e "Total tests:  $TOTAL_TESTS"
    echo -e "${GREEN}Passed:       $PASSED_TESTS${NC}"
    echo -e "${RED}Failed:       $FAILED_TESTS${NC}"
    echo -e "Success rate: ${percentage}%"
    echo ""
}

run_single_test() {
    local bin="$1"
    local test_dir="$2"
    local test_name=$(basename "$test_dir")
    
    local input="$test_dir/input.sh"
    local expected_stdout="$test_dir/expected_stdout"
    local expected_status="$test_dir/expected_status"
    
    if [ ! -f "$input" ]; then
        print_test_result "$test_name" "FAIL" "No input.sh file"
        return 1
    fi
    
    local actual_output
    actual_output=$(timeout 2 "$bin" < "$input" 2>&1)
    local actual_status=$?
    
    if [ "$actual_status" -eq 124 ]; then
        print_test_result "$test_name" "FAIL" "TIMEOUT"
        return 1
    fi
    
    local expected_output=""
    if [ -f "$expected_stdout" ]; then
        expected_output=$(cat "$expected_stdout")
    fi
    
    local expected_status_val=0
    if [ -f "$expected_status" ]; then
        # Use tr to remove any trailing newline/whitespace
        expected_status_val=$(tr -d '[:space:]' < "$expected_status")
    fi
    
    if [ "$actual_status" -eq "$expected_status_val" ] && [ "$actual_output" = "$expected_output" ]; then
        print_test_result "$test_name" "PASS"
        return 0
    else
        local details=""
        if [ "$actual_status" -ne "$expected_status_val" ]; then
            details="status: expected $expected_status_val, got $actual_status"
        else
            details="output mismatch"
        fi
        print_test_result "$test_name" "FAIL" "$details"
        return 1
    fi
}

run_category_tests() {
    local bin="$1"
    local category_dir="$2"
    local category_name=$(basename "$category_dir")
    
    local category_passed=0
    local category_total=0
    
    print_category_header "$category_name"
    
    for test_dir in "$category_dir"/*; do
        if [ -d "$test_dir" ]; then
            category_total=$((category_total + 1))
            increment_total
            
            if run_single_test "$bin" "$test_dir"; then
                category_passed=$((category_passed + 1))
                increment_passed
            else
                increment_failed
            fi
        fi
    done
    
    if [ "$category_total" -gt 0 ]; then
        print_category_summary "$category_name" "$category_passed" "$category_total"
    fi
}

cleanup() {
    rm -rf /tmp/posix-shell_test_*
}

trap cleanup EXIT