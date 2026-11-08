#!/bin/bash
# Test: everything combined 1
true && echo test || echo never; false || echo ok && true
