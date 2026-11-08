#!/bin/bash
# Test: function early return
f() { echo before; return; echo after; }; f
