#!/bin/bash
# Test: redirect close fd
echo test >&-; echo done
