#!/bin/bash
# Test: negate until
until ! false; do echo never; break; done; echo ok
