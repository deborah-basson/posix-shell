#!/bin/bash
# Test: break exits loop
while true; do echo before; break; echo after; done; echo end
