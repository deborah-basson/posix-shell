#!/bin/bash
# Test: function define in if
if true; then foo() { echo ok; }; fi; foo
