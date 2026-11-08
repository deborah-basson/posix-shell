#!/bin/bash
# Test: IFS avec espace et deux-points
IFS=" :"; var="a b:c"; echo $var
