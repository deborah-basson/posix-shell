#!/bin/bash
# Test: function pass args through
inner() { echo $1; }; outer() { inner $1; }; outer test
