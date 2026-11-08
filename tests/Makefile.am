SUBDIRS = unit

check_SCRIPTS = run_tests.sh

TESTS_ENVIRONMENT = \
	BIN_PATH="$(abs_top_builddir)/src/posix-shell" \
	COVERAGE="$(COVERAGE)" \
	OUTPUT_FILE="$(OUTPUT_FILE)"

EXTRA_DIST = \
	run_tests.sh \
	test_utils.sh \
	categories

check-local:
	@chmod +x $(srcdir)/run_tests.sh
	@chmod +x $(srcdir)/test_utils.sh 2>/dev/null || true
	@BIN_PATH="$(abs_top_builddir)/src/posix-shell" \
	 COVERAGE="$(COVERAGE)" \
	 OUTPUT_FILE="$(OUTPUT_FILE)" \
	 UNIT_TEST_DIR="$(abs_top_builddir)/tests/unit" \
	 $(srcdir)/run_tests.sh

clean-local:
	@rm -f result.txt
	@rm -rf /tmp/posix-shell_test_* 2>/dev/null || true

.PHONY: check-local clean-local
