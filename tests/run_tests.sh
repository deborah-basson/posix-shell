#!/bin/bash

set -o pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

source "$SCRIPT_DIR/test_utils.sh"

BIN_PATH="${BIN_PATH:-$SCRIPT_DIR/../src/posix-shell}"
OUTPUT_FILE="${OUTPUT_FILE:-}"
COVERAGE="${COVERAGE:-no}"
UNIT_TEST_DIR="${UNIT_TEST_DIR:-$SCRIPT_DIR/unit}"

if [ ! -f "$BIN_PATH" ]; then
    echo "Error: Binary not found at $BIN_PATH" >&2
    if [ -n "$OUTPUT_FILE" ]; then
        echo "0" > "$OUTPUT_FILE"
    fi
    exit 0
fi

init_test_counters

echo ""
echo "=========================================="
echo "  POSIX Shell Test Suite"
echo "=========================================="
echo "Binary:   $BIN_PATH"
echo "Coverage: $COVERAGE"
echo "=========================================="

START_TIME=$(date +%s)

CATEGORIES_DIR="$SCRIPT_DIR/categories"

if [ ! -d "$CATEGORIES_DIR" ]; then
    echo "Error: Categories directory not found at $CATEGORIES_DIR" >&2
    if [ -n "$OUTPUT_FILE" ]; then
        echo "0" > "$OUTPUT_FILE"
    fi
    exit 0
fi

# run a test with --pretty-print flag
run_pp_single_test()
{
    local bin="$1"
    local test_dir="$2"
    local test_name
    test_name=$(basename "$test_dir")

    local input="$test_dir/input.sh"
    local expected_stdout="$test_dir/expected_stdout"
    local expected_status_file="$test_dir/expected_status"

    if [ ! -f "$input" ]; then
        print_test_result "$test_name" "FAIL" "No input.sh file"
        return 1
    fi

    local actual_output
    actual_output=$(timeout 2 "$bin" --pretty-print < "$input" 2>/dev/null)
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
    if [ -f "$expected_status_file" ]; then
        expected_status_val=$(tr -d '[:space:]' < "$expected_status_file")
    fi

    if [ "$actual_status" -eq "$expected_status_val" ] \
        && [ "$actual_output" = "$expected_output" ]; then
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

# run all pretty_print/ category tests
run_pp_category_tests()
{
    local bin="$1"
    local category_dir="$2"
    local category_name
    category_name=$(basename "$category_dir")

    local cat_passed=0
    local cat_total=0

    print_category_header "pretty_print/$category_name"

    for test_dir in "$category_dir"/*/; do
        if [ -d "$test_dir" ]; then
            cat_total=$((cat_total + 1))
            increment_total
            if run_pp_single_test "$bin" "$test_dir"; then
                cat_passed=$((cat_passed + 1))
                increment_passed
            else
                increment_failed
            fi
        fi
    done

    if [ "$cat_total" -gt 0 ]; then
        print_category_summary "pretty_print/$category_name" \
            "$cat_passed" "$cat_total"
    fi
}

if [ "$COVERAGE" = "yes" ]; then
    echo "Mode: Coverage (unit + functional tests)"
    echo ""
    echo "=========================================="
    echo "  Unit Tests (Criterion)"
    echo "=========================================="
    echo ""

    UNIT_PASSED=0
    UNIT_FAILED=0

    if [ -d "$UNIT_TEST_DIR" ]; then
        for test_bin in "$UNIT_TEST_DIR"/test_*; do
            if [ -f "$test_bin" ] && [ -x "$test_bin" ]; then
                test_name=$(basename "$test_bin")
                echo "Running $test_name..."
                if timeout 10 "$test_bin" --verbose=0 > /dev/null 2>&1; then
                    UNIT_PASSED=$((UNIT_PASSED + 1))
                    echo "  ✓ $test_name"
                else
                    UNIT_FAILED=$((UNIT_FAILED + 1))
                    echo "  ✗ $test_name"
                fi
            fi
        done
    else
        echo "Warning: Unit test directory not found at $UNIT_TEST_DIR"
    fi

    echo ""
    echo "Unit Tests Summary:"
    echo "  Passed: $UNIT_PASSED"
    echo "  Failed: $UNIT_FAILED"
    echo "  Total:  $((UNIT_PASSED + UNIT_FAILED))"
    echo ""

    increment_passed $UNIT_PASSED
    increment_failed $UNIT_FAILED
else
    echo "Mode: Regular (functional tests only)"
    echo ""
fi

for step_dir in "$CATEGORIES_DIR"/step*; do
    if [ -d "$step_dir" ]; then
        step_name=$(basename "$step_dir")

        echo ""
        echo "=========================================="
        echo "  $step_name"
        echo "=========================================="

        for category_dir in "$step_dir"/*/; do
            if [ ! -d "$category_dir" ]; then
                continue
            fi
            category_name=$(basename "$category_dir")
            if [ "$category_name" = "pretty_print" ]; then
                run_pp_category_tests "$BIN_PATH" "$category_dir"
            elif [ -z "$PRETTY_PRINT_ONLY" ]; then
                run_category_tests "$BIN_PATH" "$category_dir"
            fi
        done
    fi
done

END_TIME=$(date +%s)
DURATION=$((END_TIME - START_TIME))

print_global_summary

echo "Execution time: ${DURATION}s"

PERCENTAGE=$(get_percentage)

if [ -n "$OUTPUT_FILE" ]; then
    echo "$PERCENTAGE" > "$OUTPUT_FILE"
    echo "Result written to: $OUTPUT_FILE"
fi

exit 0
