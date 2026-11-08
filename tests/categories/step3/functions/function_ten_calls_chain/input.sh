#!/bin/bash
# Test: function ten calls chain
a() { echo A; }; b() { a; }; c() { b; }; d() { c; }; e() { d; }; f() { e; }; g() { f; }; h() { g; }; i() { h; }; j() { i; }; j
