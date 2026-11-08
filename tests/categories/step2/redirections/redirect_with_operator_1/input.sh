#!/bin/bash
# Test: redirect with || operator
false || echo ok >or.txt; cat or.txt; rm or.txt
