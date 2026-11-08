#!/bin/bash
# Test: function status or
a() { false; }; a || echo ok
