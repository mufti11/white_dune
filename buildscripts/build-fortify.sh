#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

cd `dirname $0`/..
export CFLAGS="-D_FORTIFY_SOURCE=2 -g -O2"
export CXXFLAGS=$CFLAGS
DUNEMAKEFLAGS=-j4
export DUNEMAKEFLAGS
sh build.sh

