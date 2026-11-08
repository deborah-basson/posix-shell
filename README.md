# POSIX Shell

A POSIX-compliant shell written in **C99** with no external libraries, built as part of an **EPITA** systems programming project. The shell reads a script (from a file, a `-c` string, or standard input), tokenizes it, builds an abstract syntax tree (AST), and executes it using `fork`/`execvp`, pipes, and file descriptor redirections.

> Group project at EPITA (2026) · ~7,800 lines of C · ~3,200 functional tests

---

## Highlights

- **Classic pipeline architecture**: lexer → recursive-descent parser → AST → expansion → execution
- **Modular codebase**: each stage is a separate static library (`liblexer.a`, `libparser.a`, `libexec.a`…) linked into the final binary with Autotools
- **Strict compilation**: `-std=c99 -pedantic -Werror -Wall -Wextra -Wvla`
- **Quality tooling**: functional test suite, unit tests (Criterion), coverage reports (gcovr), and scripts that automatically check the EPITA coding style, locally or in CI
- **`--pretty-print` mode**: prints the AST instead of executing it, useful for debugging the parser

## Features

| Category | Supported |
|---|---|
| Commands | simple commands, lists (`;`, newline), comments, quoting `'…'` and `"…"` |
| Operators | pipelines `\|`, `&&`, `\|\|`, negation `!` |
| Control flow | `if / elif / else`, `while`, `until`, `for … in`, `break` |
| Grouping | `{ …; }` blocks, `( … )` subshells |
| Functions | `f() { … }` definitions, positional parameters (`$1`, `$#`, `$@`…) |
| Redirections | `>`, `>>`, `<`, `>&`, `<&`, `>\|`, with IO numbers (`2>&1`) |
| Variables | assignments, `$var`, `${var}`, special variables (`$?`, `$#`, `$$`…), environment |
| Expansion | field splitting (`IFS`), pattern matching (`*`, `?`, `[…]`) |
| Builtins | `echo`, `cd`, `exit`, `export`, `unset`, `alias`, `unalias`, `.`, `true`, `false` |

Execution modes:

```bash
./src/posix-shell -c 'echo hello | tr a-z A-Z'   # string
./src/posix-shell script.sh arg1 arg2            # file
echo 'echo hello' | ./src/posix-shell            # standard input
./src/posix-shell --pretty-print -c 'if true; then echo hi; fi'
```

Sample `--pretty-print` output:

```
list {
  if {
    condition:
      list {
        command "true"
      }
    then:
      list {
        command "echo" "hi"
      }
  }
}
```

## Architecture

```
src/
├── lexer/         Tokenization: quotes, operators, keywords, IO numbers, aliases
├── parser/        Recursive-descent parser, builds the AST
├── ast/           AST nodes (if, loops, pipeline, redirections…) and memory cleanup
├── expansion/     Variable expansion, field splitting, pattern matching
├── exec/          Execution: fork/exec, pipelines, redirections, functions, alias table
├── builtins/      Built-in commands
├── variables/     Shell variables and environment management
├── pretty_print/  AST printer
├── utils/         Error handling, helpers
└── main.c         Entry point: reads input, runs lexer → parser → exec
tests/
├── categories/    Functional tests (step1 to step4): input.sh, expected_stdout, expected_status
├── unit/          Criterion unit tests (lexer, expansion, variables, pattern matching…)
└── run_tests.sh   Functional test runner
scripts/           Build, formatting, coding-style checks, coverage
```

## Build

**Requirements**: `gcc`, `make`, `autoconf`, `automake`. Optional: `criterion` (unit tests), `gcovr` (coverage), `clang-format`.

```bash
# Debian / Ubuntu
sudo apt install build-essential autoconf automake libcriterion-dev gcovr clang-format
```

```bash
./scripts/compile.sh     # autoreconf + configure + make
# or manually:
autoreconf --install && ./configure && make
```

The binary is built at `src/posix-shell`.

Clean up: `./scripts/clean.sh`

## Tests

```bash
BIN_PATH="$(pwd)/src/posix-shell" ./tests/run_tests.sh
```

Each functional test is a directory containing the script to run (`input.sh`), the expected output (`expected_stdout`), and the expected exit code (`expected_status`). Tests are grouped by project step and feature, with results reported per category.

### Coverage

```bash
./scripts/coverage-report.sh         # generate the report (gcovr)
./scripts/coverage-report.sh clean   # remove generated files
```

## Code quality

### Coding-style checks (CI)

```bash
./scripts/run-pipeline-checks-locally.sh
```

Runs locally the same checks as the CI pipeline. They enforce the EPITA C coding style:

- max 10 exported functions and 1 exported variable per file
- max 30 lines and 4 parameters per function
- explicit `void` for parameterless prototypes
- `*` attached to the variable name, not the type
- explicit dereference when calling function pointers
- no `typedef` on `struct` definitions
- no tabs (space indentation), trailing newline, closing comment in `.h` files

### Formatting

```bash
./scripts/clang-format.sh
```

Runs `clang-format`, then automatically fixes a flaw in the provided configuration (the placement of `*` inside function-pointer parentheses) so the code stays compliant with the coding style.

## Project status and known limitations

Steps 1 to 3 of the specification are mostly implemented. Not yet supported:

- command substitution (`$(…)` and `` `…` ``)
- `case … esac`
- some advanced variable expansion and field splitting edge cases

## Skills demonstrated

Low-level C and manual memory management · POSIX system calls (`fork`, `execvp`, `waitpid`, `pipe`, `dup2`, `open`) · lexer and parser design · ASTs · Autotools · automated testing and CI · teamwork on a shared codebase
