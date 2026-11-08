#!/bin/bash
# Test: if elif status
if false; then true; elif true; then false; fi; echo $?
