#!/bin/bash
# Test: status after if
if true; then false; fi; echo $?
