#!/bin/bash
# Test: deep mixed nesting
({ (echo level3); }; echo level2)
