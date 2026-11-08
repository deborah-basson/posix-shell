#!/bin/bash
# Test: block in block
{ { echo nested; }; echo outer; }
