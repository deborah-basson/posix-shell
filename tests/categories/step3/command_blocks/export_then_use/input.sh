#!/bin/bash
# Test: export then use
VAR=local && export VAR && echo $VAR
